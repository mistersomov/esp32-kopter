#include "manager/MessageManager.hpp"
#include "Message.hpp"
#include "Task.hpp"

#include "esp_log.h"
#include "gpio_cxx.hpp"

#include <string_view>

#define WIFI_CHANNEL CONFIG_AP_WIFI_CHANNEL
#define ESPNOW_QUEUE_SIZE 6
#if CONFIG_WIFI_MODE_SOFTAP
#define ESPNOW_WIFI_IF WIFI_IF_AP
#else
#define ESPNOW_WIFI_IF WIFI_IF_STA
#endif

// Customize region begin
#if CONFIG_WIFI_MODE_SOFTAP
static uint8_t dest_mac[ESP_NOW_ETH_ALEN] = {0x2c, 0xbc, 0xbb, 0x92, 0xf7, 0xa8};
#else
static uint8_t dest_mac[ESP_NOW_ETH_ALEN] = {0xc8, 0xf0, 0x9e, 0xb2, 0x36, 0xfd};
#endif
// Customize region end

namespace kopter {

static constexpr std::string_view MESSAGE_TASK_NAME = "msg_task";
static constexpr std::string_view TAG = "[MessageManager]";

MessageManager::MessageManager()
{
}

MessageManager::~MessageManager()
{
    if (m_receive_task) {
        delete m_receive_task;
        m_receive_task = nullptr;
    }
    if (m_queue) {
        vQueueDelete(m_queue);
        m_queue = nullptr;
    }
    ESP_ERROR_CHECK(esp_now_deinit());
}

MessageManager &MessageManager::get_instance()
{
    static MessageManager instance;
    return instance;
}

void MessageManager::init()
{
    m_queue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(Message));
    if (m_queue == nullptr) {
        ESP_LOGE(TAG.data(), "Creating messages queue fail");
        return;
    }

    s_callback_instance = this;

    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(esp_now_send_cb));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(esp_now_recv_cb_forwarder));
    ESP_ERROR_CHECK(add_peer_to_list());

    m_receive_task = new Task(MESSAGE_TASK_NAME.data(), [this]() {
        Message msg;
        auto led = idf::GPIO_Output(idf::GPIONum(2));

        while (true) {
            if (xQueueReceive(m_queue, &msg, portMAX_DELAY)) {
                ESP_LOGI(TAG.data(), "Received msg id=%d", static_cast<int>(msg.sensor_id));
                if (msg.sensor_id == SensorID::PITCH) {
                    led.set_high();
                }
                else if (msg.sensor_id == SensorID::ROLL) {
                    led.set_low();
                }
            }
        }
    });
}

void MessageManager::send_message(Message *msg) const
{
    uint8_t buffer[Message::size()];
    msg->serialize(buffer);

    esp_err_t result = esp_now_send(dest_mac, buffer, sizeof(buffer));
    if (result == ESP_OK) {
        ESP_LOGI(TAG.data(), "Message sent successfully");
    }
    else {
        ESP_LOGE(TAG.data(), "Failed to send message: %d\n", result);
    }
}

void MessageManager::esp_now_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
}

void MessageManager::esp_now_recv_cb_forwarder(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
    if (MessageManager::s_callback_instance) {
        MessageManager::s_callback_instance->handle_recv(info, data, len);
    }
}

void MessageManager::handle_recv(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
    if (len < Message::size()) {
        ESP_LOGE(TAG.data(), "Error length received data");
        return;
    }

    Message msg = Message::deserialize(data);
    BaseType_t task_woken = pdFALSE;

    xQueueSendFromISR(m_queue, &msg, &task_woken);

    if (task_woken) {
        portYIELD_FROM_ISR();
    }
}

esp_err_t MessageManager::add_peer_to_list() const
{
    esp_now_peer_info peer_info{};
    peer_info.channel = WIFI_CHANNEL;
    peer_info.ifidx = ESPNOW_WIFI_IF;
    peer_info.encrypt = false;
    std::copy(std::begin(dest_mac), std::end(dest_mac), peer_info.peer_addr);

    return esp_now_add_peer(&peer_info);
}

} // namespace kopter