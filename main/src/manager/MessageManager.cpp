#include "manager/MessageManager.hpp"
#include "Message.hpp"

#include "esp_log.h"
#include "esp_mac.h"
#include "esp_now.h"
#include <string_view>

#define WIFI_CHANNEL CONFIG_AP_WIFI_CHANNEL
#if CONFIG_WIFI_MODE_SOFTAP
#define ESPNOW_WIFI_IF WIFI_IF_AP
#else
#define ESPNOW_WIFI_IF WIFI_IF_STA
#endif

namespace kopter {

constexpr std::string_view TAG = "[MessageManager]";

static void esp_now_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
}

static void esp_now_rv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len)
{
}

MessageManager::MessageManager()
{
}

MessageManager::~MessageManager()
{
    ESP_ERROR_CHECK(esp_now_deinit());
}

MessageManager &MessageManager::get_instance()
{
    static MessageManager instance;
    return instance;
}

void MessageManager::init()
{
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(esp_now_send_cb));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(esp_now_rv_cb));
    ESP_ERROR_CHECK(add_broadcast_peer());
}

void MessageManager::send_message(Message *msg) const
{
    uint8_t buffer[Message::size()];
    msg->serialize(buffer);

    ESP_LOGI(TAG.data(), "Sending data: %d", msg->data);
    // отправка по ESP-NOW
    esp_err_t result = esp_now_send(recv_mac, buffer, sizeof(buffer));
    if (result == ESP_OK) {
        ESP_LOGI(TAG.data(), "Message sent successfully");
    }
    else {
        ESP_LOGE(TAG.data(), "Failed to send message: %d\n", result);
    }
}

esp_err_t MessageManager::add_broadcast_peer()
{
    esp_now_peer_info peer_info{};
    peer_info.channel = WIFI_CHANNEL;
    peer_info.ifidx = ESPNOW_WIFI_IF;
    peer_info.encrypt = false;

    std::memcpy(peer_info.peer_addr, recv_mac, sizeof(recv_mac));
    return esp_now_add_peer(&peer_info);
}

} // namespace kopter