1. インクルードガードは#define～#endifで実装する。
2. 変数はスネークケース、関数はキャメルケースで命名する。
3. クラス名はパスカルケースで命名する。
4. 改行については以下のルールを守る。
   - 関数定義の前後には1行の空行を入れる。
   - クラス定義の前後には1行の空行を入れる。
   - if文やループ文の前後には1行の空行を入れる。
   - 論理的な区切りごとに1行の空行を入れる。
   - elseやelse ifは改行する
5. インデントはスペース4つ分とする。
6. コメントは日本語で記述する
7. クラスについては以下のように書く
```cpp
#ifndef CLASS_NAME_HPP
#define CLASS_NAME_HPP

class ClassName {

    public:
    
        // コンストラクタ
        ClassName();

        // デストラクタ
        virtual ~ClassName();

    private:
        // メンバ変数
        int member_variable_;

        // メンバ関数
        void memberFunction();
};