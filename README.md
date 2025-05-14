# frida-clr.net-patch

[![.NET](https://img.shields.io/badge/.NET-9.0-blue.svg)](https://dotnet.microsoft.com/download/dotnet/9.0)
![Platform](https://img.shields.io/badge/Support%20Platform-Windows-blue.svg)
[![License](https://img.shields.io/badge/License-wxWidgets-blue.svg)](https://github.com/sht2017/frida-clr.net-patch/blob/main/COPYING)
[![Latest Release](https://img.shields.io/github/v/release/sht2017/frida-clr.net-patch?label=Latest%20Release)](https://github.com/sht2017/frida-clr.net-patch/releases/latest)
[![Generate Patch](https://github.com/sht2017/frida-clr.net-patch/actions/workflows/CI.yaml/badge.svg)](https://github.com/sht2017/frida-clr.net-patch/actions/workflows/CI.yaml)

Automatically generates patches for Frida CLR (.NET) bindings when new Frida versions are released.

## Overview

This project maintains up-to-date patches for the Frida CLR bindings, allowing .NET applications to use the latest Frida features. It automatically checks for new Frida releases daily and generates corresponding patch files.

The patches migrate Frida CLR from the legacy .NET Framework to modern .NET, enabling better cross-platform compatibility and modern .NET features. WPF specific features have been removed.

## Features

- Automatic daily checks for new Frida releases
- Patch generation for new versions
- Support for .NET 9.0
- Migrated from WPF dependencies to system-agnostic rendering

## Usage

...

## Todo
- [ ] ~~Add support for other platforms~~ **No plan for now, open to PRs**

## Acknowledgements
- [frida](https://frida.re/) - The dynamic instrumentation toolkit for developers, reverse-engineers, and security researchers.
- [frida-clr](https://github.com/frida/frida-clr) - The CLR (.NET) bindings for Frida. Most work is based on this repo.