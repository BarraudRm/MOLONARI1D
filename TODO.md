# Todo, short term

- change **hardware/sensors/temperature/Sensor** in **hardware/sensors/temperature/Main_Sensor** or smth *more relevent*
- update **demo readme** (update directories description) 
- check if **Relay_LoRaWan/internals/** scripts are ever called ; if so integrate sctipts in **shared/**, if not discard **Relay_LoRaWan/internals/**
- git checkout previous commit to do the same as before with **Sensor_demo/internals/** 
- update **readme** files of modifyied folders. Possibly nonexhaustive list : **mains_ino/sensor/readme.md** ; 
- move **shared/** in **common/**
- fix field_code/mains_ino/relay/ **Relay.ino** and **Relay_LoRaWan.ino** names and purpose
- fix architecture **libs/vrac/**
- handle **hardware/tests/**, decide what to keep and what to discard



# What we should have :
```
hardware/field_code          # opposed to backend or data analysis code
│                
├── mains_ino/               # directory ONLY USED FOR .INO FILES
│   ├── sensor/              # sensor ino code (codes if demo phase)
│   ├── relay/               # relay ino code (codes if demo phase)
│   └── gateway/             # gateway ino code (codes if demo phase)
|
└── libs/                    # Scientific computing engine
    ├── shared/              # General utilities
    |   ├── protocols/       # Communicaiton libraries
    |   |   ├── LoRa         # LoRa C++ code
    |   |   └── LoRaWan      # LoRaWan C++ code
    |   └── ...              # some more if needeed
    |                     
    ├── sensors/              # libraries only useful for sensors
    |   ├── sensors_common
    |   ├── temperature
    |   └── pressure
    ├── relay/               # libraries only useful for relay
    └── gateway/             # libraries only useful for gateway
```
# Done : 

- changed architecture to match above scheme
  
- trashed old **/internals** redundant codes in **trash**
  
- **hardware/sensors/temperature/Sensor/Sensor.ino** became hardware/**field_code/libs/sensors/temperature/temperature_sensor.ino**
  
- 

