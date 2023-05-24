# Notification System (Chapter10)
ニュース速報や予定などを通知するシステム．いくつかの種類が存在する
![image](https://github.com/melonoidz/system_design_note/assets/27326835/4e53f0b7-c313-441b-8e9b-73c0ac3b56ef)

## Design Process Framework
### Step1: 課題理解，スコープ定義
- どのタイプをサポートする？
  - Push通知，SMS，Email
#### フェルミ推定
### Step2:基本設計
#### Different types of notifications
##### iOS push notification
![image](https://github.com/melonoidz/system_design_note/assets/27326835/34debe9e-a2dd-4fd4-9eb1-0e921194a7f8)

```bash
{
 "aps":{
         "alert":{
           "title": "Game Request",
           "body" : "Bob wants to play chess",
           "action-loc-key": "PLAY"
         },
         "badge":5
       }
}

```

##### Android push notification
![image](https://github.com/melonoidz/system_design_note/assets/27326835/5ffe1553-e6b0-4362-b3af-1ac696210059)
##### SMS message
![image](https://github.com/melonoidz/system_design_note/assets/27326835/ca0332bf-33e0-4e76-9bbd-d44a0149f67f)

##### Email
![image](https://github.com/melonoidz/system_design_note/assets/27326835/cfd87120-41fd-486d-9bbc-023781372b59)

##### All including conclusion
![image](https://github.com/melonoidz/system_design_note/assets/27326835/cd6d2ed6-48f5-450e-a8fa-c633034e9784)

#### Contact info gathering flow
![image](https://github.com/melonoidz/system_design_note/assets/27326835/7b3f3f35-b462-4e4d-a799-ddef6e0dc2a0)
![image](https://github.com/melonoidz/system_design_note/assets/27326835/65a90e4e-6f30-4894-8137-4fbeea0fa083)

#### Notification sending/receiving flow
##### High-level design
![image](https://github.com/melonoidz/system_design_note/assets/27326835/e40c2746-f016-4323-b09d-ca732e0e72ce)

###### Service 1 to N
###### Notification system
###### Third-party services
###### iOS, Android, SMS, Email

##### High-level design(improved)
![image](https://github.com/melonoidz/system_design_note/assets/27326835/3ccb02f6-80eb-41c1-bd79-15e7fd50ffe7)

###### Service 1 to N
###### Notification servers
```bash
{
 "to":[
   {
     "user_id": 123456
   }
 ],
 "from": {
   "email": "from_address@example.com"
 },
 "subject": "Hello, World!",
 "content": [
   {
     "type": "text/plain",
     "value": "Hello, World!"
   }
 ]

}
```
###### Cash
###### DB
###### Workers
###### Third-party services
###### iOS, Android, SMS, Email

### Step3:詳細設計
#### Reliability
##### データロスを防ぐには？
![image](https://github.com/melonoidz/system_design_note/assets/27326835/0615b669-010f-4eb9-8ee4-91a9c9fb7904)



### Step4:まとめ
#### Extra
