//Required HTTPClientESP32Ex library, can be installed from https : //github.com/mobizt/HTTPClientESP32Ex

//To use send Email for Gmail, less secure app option should be enabled. https://myaccount.google.com/lesssecureapps?pli=1

//To receive Email for Gmail, IMAP option should be enabled. https://support.google.com/mail/answer/7126229?hl=en
#include <Arduino.h>
#include "ESP32_MailClient.h"
#include "SD.h"

//For demo only
#include "image.h"

#define WIFI_SSID "HUAWEI-B315-A94D"
#define WIFI_PASSWORD "TA2E10M1GQA"

//WiFi or HTTP client for internet connection
HTTPClientESP32Ex http;

//The Email Sending data object contains config and data to send
SMTPData smtpData;

//Callback function to get the Email sending status
void sendCallback(SendStatus info);

void setup()
{

  MailClient.sdBegin(14,2,15,13);

  Serial.begin(115200);
  Serial.println();

  Serial.print("Connecting to AP");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println();
  Serial.println("Sending email...");

  //Set the Email host, port, account and password
  smtpData.setLogin("smtp.gmail.com", 465, "odysseas.tests@gmail.com", "hackcyprus2019");

  //Set the sender name and Email
  smtpData.setSender("LamaGuard", "lamaguard@robo.com.cy");

  //Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  //Set the subject
  smtpData.setSubject("Security Breach!");

  //Set the message - normal text or html format
  smtpData.setMessage("<div style=\"color:#ff0000;font-size:20px;\">Security breach on your warehouse located at Evelthontos 18A Nicosia!</div> <div style=\"font-size:18px;\">The following photos where taken by the security camera:</div>", true);

  //Add recipients, can add more than one recipient
  smtpData.addRecipient("odysseas.tests@gmail.com");



  //Add attachments, can add the file or binary data from flash memory, file in SD card
  //Data from internal memory
  //smtpData.addAttachData("firebase_logo.png", "image/png", (uint8_t *)file, sizeof file);

  //Set the storage types to read the attach files (SD is default)
  //smtpData.setFileStorageType(MailClientStorageType::SPIFFS);
  smtpData.setFileStorageType(MailClientStorageType::SD);

  //Add attach files from SD card
  //Comment these two lines, if no SD card connected
  //Two files that previousely created.
//  smtpData.addAttachFile("/binary_file.dat");
  smtpData.addAttachFile("/picture1.png");
  smtpData.setFileStorageType(MailClientStorageType::SD);

  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(http, smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();

}

void loop()
{
}

//Callback function to get the Email sending status
void sendCallback(SendStatus msg)
{
  //Print the current status
  Serial.println(msg.info());

  //Do something when complete
  if (msg.success())
  {
    Serial.println("---LamaGuard---");
  }
}
