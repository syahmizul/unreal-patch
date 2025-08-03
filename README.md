# Unreal Engine Patcher

Patches some bytes and hooks some functions in order to do some stuff that's normally not allowed by some program..


## Installation

1. Make sure your account have access to Unreal Engine's source code.
2. Clone RE-UE4SS repo into the root path.
3. `cd` into RE-UE4SS and execute: `git submodule update --init --recursive`
4. Then execute this at the root path :
```bash
xmake f -m "Game__Shipping__Win64"
xmake
```

or 

```bash
xmake project -k vsxmake2022
```