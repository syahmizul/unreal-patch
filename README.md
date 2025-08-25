# Unreal Engine Asset Signature Bypass

UE4SS mod that bypasses Unreal Engine asset signature checks.

## Compatibility

### Tested Versions
- Unreal Engine 5.5.4

## Prerequisites

- Epic Games account with Unreal Engine source code access
- XMake
- Git

## Installation

### 1. Clone and Setup
```bash
git clone https://github.com/syahmizul/unreal-patch.git
cd unreal-patch
git submodule update --init --recursive
```

### 2. Build Options

#### Direct Build
```bash
xmake f -m "Game__Shipping__Win64"
xmake
```

#### Generate Visual Studio Project
```bash
xmake project -k vsxmake2022
```
Then open the generated solution in Visual Studio 2022.

## Credits

- [Buckminsterfullerene02 / LongerWarrior](https://gist.github.com/Buckminsterfullerene02/90077ce81c0fd908144498869f4ea288)
