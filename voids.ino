
void send_the_picture() {
  digitalWrite(33, LOW);          // light on
  currentByte = 0;
  fb_length = vid_fb->len;
  fb_buffer = vid_fb->buf;

  Serial.println("\n>>>>> Sending as 512 byte blocks, with jzdelay of 0, bytes=  " + String(fb_length));

  if (active_interupt) {
    String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
                  "image/jpeg", "PIR Event!", chat_id, fb_length,
                  isMoreDataAvailable, getNextByte, nullptr, nullptr);
  } else {
    String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
                  "image/jpeg", "Telegram Request", chat_id, fb_length,
                  isMoreDataAvailable, getNextByte, nullptr, nullptr);
  }
  esp_camera_fb_return(vid_fb);
  bot.longPoll =  0;
  digitalWrite(33, HIGH);          // light oFF
  if (!avi_enabled) active_interupt = false;
}

void send_the_video() {
  digitalWrite(33, LOW);          // light on
  Serial.println("\n\n\nSending clip with caption");
  Serial.println("\n>>>>> Sending as 512 byte blocks, with a caption, and with jzdelay of 0, bytes=  " + String(psram_avi_ptr - psram_avi_buf));
  avi_buf = psram_avi_buf;

  avi_ptr = 0;
  avi_len = psram_avi_ptr - psram_avi_buf;

  String sent2 = bot.sendMultipartFormDataToTelegramWithCaption("sendDocument", "document", strftime_buf,
                 "image/jpeg", "Intruder alert!", chat_id, psram_avi_ptr - psram_avi_buf,
                 avi_more, avi_next, nullptr, nullptr);

  Serial.println("done!");
  digitalWrite(33, HIGH);          // light off

  bot.longPoll = 5;
  active_interupt = false;
}
