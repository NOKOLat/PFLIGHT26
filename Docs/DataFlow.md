## 1. センサーデータの取得

- ここでは、vector3fという構造体を使用しています
- ゲッター・セッターの他に、enumを使用して軸を指定する方法もあります

### Vector3fの説明
```cpp
Vector3f data;
data.x() = 1.0f; // 名前付きアクセサで値を設定
data[Axis::Y] = 2.0f; // enumを使用して値を設定

float x = data.x(); // 名前付きアクセサで値を取得
float y = data[Axis::Y]; // enumを使用して値を取得
```

### 実際のセンサーデータ
```cpp
Vector3f accel;
accel.x() = x; // 加速度センサーのデータ
accel.y() = y;
accel.z() = z;

Vector3f gyro;
gyro.x() = x; // ジャイロセンサーのデータ
gyro.y() = y;
gyro.z() = z;
```

## 2. 姿勢推定

- EKFを使って6軸IMUのデータから姿勢を推定します
- 入力はセンサーデータで、出力はEuler3fという構造体に保存しています

### Euler3fの説明

- 基本的にVector3fと同様の使い方ができます
- ただし、Euler3fは姿勢を表すための構造体で、roll, pitch, yawの値を格納します
```cpp
Euler3f data;
data.roll() = 1.0f; // 名前付きアクセサで値を設定
data[Axis::PITCH] = 2.0f; // enumを使用して値を設定

float roll = data[Axis::ROLL]; // enumを使用して値を取得
float pitch = data.pitch(); // 名前付きアクセサで値を取得
```

## 3. 高度推定

- 加速度、気圧センサー、姿勢推定の結果を使用して高度を推定します
- 出力はライブラリで定義されているので、必要な高度だけを受け取ります

```cpp
context.instances.altitude_estimator->Update(context.sensor_data.barometric_pressure, context.sensor_data.accel.getptr(), angle_vec.getptr(), context.loop_time_us / 1000000.0f);

// 高度推定結果を取得
float altitude_data[3];  // [altitude, velocity, accel]
context.instances.altitude_estimator->GetData(altitude_data);
context.attitude_state.altitude = altitude_data[0];
```


## 4. PWM出力
- PWM出力は、モーターとサーボで独立した構造体を使用しています

### MotorPWMの説明
- right()とleft()の関数を使用して、右と左のモーターのPWM値を設定します
- また、enumを使用して左右を指定することもできます
```cpp
// モーターの出力[%]を指定
context.control_output.motor_pwm.right()           = 15.0f;
context.control_output.motor_pwm[MotorIndex::LEFT] = 15.0f;
```

### ServoPWMの説明
- elevator(), rudder(), aileron()の関数を使用して、エレベーター、ラダー、エルロンのPWM値を設定します
- また、enumを使用して各サーボを指定することもできます
```cpp
// サーボの角度[deg]を指定
context.control_output.servo_pwm.elevator()            = 25.0f;
context.control_output.servo_pwm[ServoIndex::RUDDER]   = 15.0f;
```