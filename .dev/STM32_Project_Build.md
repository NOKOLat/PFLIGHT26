# STM32 CubeIDE プロジェクトビルドガイド

## 概要
STM32CubeIDEのプロジェクトをコマンドラインからビルドする汎用的な方法です。IDE実行中でも並行ビルドが可能です。

## 環境情報
エラーが連続する場合は、ユーザーに確認を促してください。
ユーザーにパスを貼るように要求し、ここを直接書き換えて構いません
- **STM32CubeIDE**: D:\ST\STM32CubeIDE_1.19.0
- **ワークスペース**: d:\GitLocal\STM32CubeIDE

## 必須ステップ

### ステップ1：初回セットアップ - Debugフォルダ作成
新しいプロジェクトをビルドする場合、まずSTM32CubeIDEでプロジェクトを開いてDebugフォルダを自動生成させます。

```powershell
# STM32CubeIDEを起動（対象プロジェクトを開く）
# IDE内でビルド（Ctrl+B）を1回実行 → Debug/フォルダが作成される
# その後、IDEを閉じてコマンドラインビルドへ移行
```

**IDE実行中でも可能な場合**：
```powershell
& "D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\stm32cubeidec.exe" `
  --launcher.suppressErrors `
  -nosplash `
  -application org.eclipse.cdt.managedbuilder.core.headlessbuild `
  -data "d:\GitLocal\STM32CubeIDE" `
  -build "プロジェクト名/Debug"
```

### ステップ2：コマンドラインビルド

環境変数を設定して、makeを実行します。

```powershell
$make_path = "D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.0.202409170845\tools\bin"
$gnu_tools_path = "D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin"

$env:PATH = "$gnu_tools_path;$make_path;" + $env:PATH

$project_name = "PFLIGHT_26"  # または他のプロジェクト名
$debug_dir = "d:\GitLocal\STM32CubeIDE\$project_name\Debug"

& "$make_path\make.exe" -C "$debug_dir" all -j24
```

## ビルド結果の確認

ビルド成功時の出力例：　（ここではPFLIGHT_26プロジェクトを例示）
```
make: Entering directory 'd:/GitLocal/STM32CubeIDE/PFLIGHT_26/Debug'
arm-none-eabi-size  PFLIGHT_26.elf
   text    data     bss     dec     hex filename
  53360     468    3876   57704    e168 PFLIGHT_26.elf
Finished building: default.size.stdout
make: Leaving directory 'd:/GitLocal/STM32CubeIDE/PFLIGHT_26/Debug'
```

成果物：
- `*.elf` - 実行可能ファイル
- `*.map` - メモリマップファイル
- `*.list` - 逆アセンブルリスト

## トラブルシューティング

### エラー: "No such file or directory"
→ プロジェクトの Debug ディレクトリが存在しません。ステップ1を実行してください。

### エラー: "arm-none-eabi-size Error -1"
→ PATH環境変数にGNUツール パスが設定されていません。ステップ2の環境変数設定を確認してください。

### IDE実行中のビルド競合
→ IDE実行中は新しいワークスペース(-data)を指定するか、IDEを閉じてからビルドしてください。

## スクリプト化

複数プロジェクトを一括ビルドしたい場合は、以下をPowerShellスクリプトとして保存：

```powershell
# build_all_projects.ps1
param(
    [string[]]$Projects = @("PFLIGHT_26", "F446_DPS368", "F446_EKF")
)

$make_path = "D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.0.202409170845\tools\bin"
$gnu_tools_path = "D:\ST\STM32CubeIDE_1.19.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344\tools\bin"

$env:PATH = "$gnu_tools_path;$make_path;" + $env:PATH

foreach ($project in $Projects) {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "Building: $project" -ForegroundColor Yellow
    Write-Host "========================================" -ForegroundColor Cyan
    
    $debug_dir = "d:\GitLocal\STM32CubeIDE\$project\Debug"
    & "$make_path\make.exe" -C "$debug_dir" all -j24
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✅ $project build succeeded" -ForegroundColor Green
    } else {
        Write-Host "❌ $project build failed" -ForegroundColor Red
    }
}
```

## 実行方法

```powershell
# カレントプロジェクトをビルド
$project_name = "PFLIGHT_26"
$debug_dir = "d:\GitLocal\STM32CubeIDE\$project_name\Debug"
& make -C "$debug_dir" all -j24

# または別プロジェクト
$project_name = "F446_DPS368"
```

---
**最終更新**: 2026年2月3日
**検証済みバージョン**: STM32CubeIDE v1.19.0
