---
name: stm32_build
description: Build STM32 firmware using make
---

# STM32CubeIDE ビルドスキル

STM32CubeIDEプロジェクトをコマンドラインからビルドします。

## クイックスタート

以下を実行してビルドしてください（Git Bash推奨）：

```bash
export PATH="D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin:D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.0.202409170845\tools\bin:$PATH" && cd d:/GitLocal/STM32CubeIDE/PFLIGHT_26/Debug && make clean && make all -j24
```

✅ エラーなく `Finished building: PFLIGHT_26.list` が出力されたらビルド成功

## 環境情報

- **STM32CubeIDE**: D:\ST\STM32CubeIDE_1.19.0
- **ワークスペース**: d:\GitLocal\STM32CubeIDE
- **プロジェクト**: PFLIGHT_26

## ビルド成果物

- `PFLIGHT_26.elf` - 実行可能ファイル
- `PFLIGHT_26.map` - メモリマップ
- `PFLIGHT_26.list` - 逆アセンブルリスト

## トラブルシューティング

| エラー | 対処 |
|--------|------|
| `No such file or directory` | プロジェクトで一度IDEビルドを実行し、Debugフォルダを生成 |
| `arm-none-eabi-size Error -1` | PATH設定が不正。上記クイックスタートを再実行 |
| IDE実行中のビルド競合 | IDEを閉じるか、別のワークスペースを指定 |

## 参考：ビルドスクリプト

プロジェクトルートに `build.sh` を保存して実行：

```bash
#!/bin/bash
set -e

MAKE_PATH="D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.0.202409170845\tools\bin"
GNU_TOOLS_PATH="D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin"

export PATH="${GNU_TOOLS_PATH}:${MAKE_PATH}:$PATH"

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_DIR/Debug"
make clean && make all -j24

echo "✅ Build completed: $PROJECT_DIR/Debug/PFLIGHT_26.elf"
```

---
**最終更新**: 2026年2月26日 | **STM32CubeIDE**: v1.19.0
