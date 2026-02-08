input uint16_t sbus_data[18];

output sbus_data (struct)

## struct sbus_data 

float throttle (0~100)
float aileron (-100~100)
float elevator (-100~100)
float rudder (-100~100)
uint8_t aux1 (0 or 1)
uint8_t aux2 (0 or 1)
uint8_t aux3 (0 or 1)
uint8_t aux4 (0 or 1)
uint8_t aux5 (0 or 1)
uint8_t aux6 (0 or 1)

enum classでアクセスできるようにする
static_castをしなくていいように演算子オーバーロードしといてください

変換関数にデータを入れると一括ですべての変換を行ってstructに詰めて返すようにしてください
staticなメソッドになりますが、float型の各値に変換するデータのmin max centerとauxの閾値は設定用の構造体をクラス内にもってください

構造体はstate_contextに新たに加えて、statemanagerのsbus受信処理の直後に変換関数を呼び出して変換したデータを格納してください