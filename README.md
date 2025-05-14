# frida-clr.net-patch

[![.NET](https://img.shields.io/badge/.NET-9.0-blue.svg)](https://dotnet.microsoft.com/download/dotnet/9.0)
![Platform](https://img.shields.io/badge/Support%20Platform-Windows-blue.svg)
[![License](https://img.shields.io/badge/License-wxWidgets-blue.svg)](https://github.com/sht2017/frida-clr.net-patch/blob/main/COPYING)
[![Generate Patch](https://github.com/sht2017/frida-clr.net-patch/actions/workflows/CI.yaml/badge.svg)](https://github.com/sht2017/frida-clr.net-patch/actions/workflows/CI.yaml)
[![Latest Release](https://img.shields.io/github/v/release/sht2017/frida-clr.net)](https://github.com/sht2017/frida-clr.net/releases/latest)

Automatically generates patches for Frida CLR (.NET) bindings when new Frida versions are released.

## Overview

This project maintains up-to-date patches for the Frida CLR bindings, allowing .NET applications to use the latest Frida features. It automatically checks for new Frida releases daily and generates corresponding patch files.

The patches migrate Frida CLR from the legacy .NET Framework to modern .NET, enabling better cross-platform compatibility and modern .NET features. WPF specific features have been removed.

I started this project because I would like to use Frida in my **Steam Pal** project (haven't released yet), which is an extension about convenient searching and managing the library for Command Palette of PowerToys. Somehow, I found that the Frida CLR bindings are strongly related to WPF, which doesn't fit the specific purpose of my project. So I decided to migrate the Frida CLR bindings to modern .NET and remove the WPF dependencies.

## Features

- Automatic daily checks for new Frida releases
- Patch generation for new versions
- Support for .NET 9.0
- Migrated from WPF dependencies to system-agnostic rendering

## Build

Go check [CI](https://github.com/sht2017/frida-clr.net/blob/main/.github/workflows/CI.yml).

## Usage
1. Download the [latest release](https://github.com/sht2017/frida-clr.net/releases/latest) with your targeted architecture.
2. Import the `Frida.dll` into your project.
3. Enjoy!

## Todo
- [ ] ~~Add support for other platforms~~ **No plan for now, open to PRs**

## Acknowledgements
- [frida](https://frida.re/) - The dynamic instrumentation toolkit for developers, reverse-engineers, and security researchers.
- [frida-clr](https://github.com/frida/frida-clr) - The CLR (.NET) bindings for Frida. Most work is based on this repo.