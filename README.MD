# ws-patcher

## What is this?

A patcher that patches warspear.exe to enable multiboxing.

## How to use it?

```bash

./ws-patcher warspear.exe

```

## How it works?

In general, this program searches for the call to `CreateMutex`,and change the control flow.
