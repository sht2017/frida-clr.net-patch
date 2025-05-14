import os
from pathlib import Path
import sys
import winreg
from typing import Optional

NET_VERSION = "9.0"

def main():
    sdk_location = find_sdk()
    if sdk_location is None:
        print(f".NET SDK {NET_VERSION} is not installed", file=sys.stderr)
        sys.exit(1)

    netfxsdk_location = find_netfxsdk()
    if netfxsdk_location is None:
        print(".NET Framework SDK is not installed", file=sys.stderr)
        sys.exit(1)

    runtime_location = find_runtime()
    if runtime_location is None:
        print(f".NET Runtime {NET_VERSION} is not installed", file=sys.stderr)
        sys.exit(1)

    desktop_runtime_location = find_desktop_runtime()
    if desktop_runtime_location is None:
        print(f".NET Desktop SDK {NET_VERSION} is not installed", file=sys.stderr)
        sys.exit(1)

    print(str(sdk_location))
    print(str(netfxsdk_location))
    print(str(runtime_location))
    print(str(desktop_runtime_location))

def find_runtime() -> Optional[Path]:
    bases = [
        Path(os.environ.get("ProgramFiles", "")) / "dotnet",
        Path(os.environ.get("ProgramFiles(x86)", "")) / "dotnet"
    ]
    for base in bases:
        shared = base / "shared" / "Microsoft.NETCore.App"
        if not shared.is_dir():
            continue
        versions = [d.name for d in shared.iterdir()
                    if d.is_dir() and d.name.startswith(f"{NET_VERSION}.")]
        if versions:
            versions.sort(key=lambda v: [int(x) for x in v.split(".")])
            return shared / versions[-1]
    return None

def find_desktop_runtime() -> Optional[Path]:
    bases = [
        Path(os.environ.get("ProgramFiles", "")) / "dotnet",
        Path(os.environ.get("ProgramFiles(x86)", "")) / "dotnet"
    ]
    for base in bases:
        shared = base / "shared" / "Microsoft.WindowsDesktop.App"
        if not shared.is_dir():
            continue
        versions = [d.name for d in shared.iterdir()
                    if d.is_dir() and d.name.startswith(f"{NET_VERSION}.")]
        if versions:
            versions.sort(key=lambda v: [int(x) for x in v.split(".")])
            return shared / versions[-1]
    return None

def find_sdk() -> Optional[Path]:
    bases = [
        Path(os.environ.get("ProgramFiles", "")) / "dotnet",
        Path(os.environ.get("ProgramFiles(x86)", "")) / "dotnet"
    ]
    for base in bases:
        sdk = base / "sdk"
        if not sdk.is_dir():
            continue
        versions = [d.name for d in sdk.iterdir()
                    if d.is_dir() and d.name.startswith(f"{NET_VERSION}.")]
        if versions:
            versions.sort(key=lambda v: [int(x) for x in v.split(".")])
            return sdk / versions[-1]
    return None

def find_netfxsdk() -> Optional[Path]:
    roots = ["WOW6432Node\\", ""]
    for root in roots:
        try:
            netfxsdk_base = f"SOFTWARE\\{root}Microsoft\\Microsoft SDKs\\NETFXSDK\\"
            key = winreg.OpenKey(
                winreg.HKEY_LOCAL_MACHINE,
                netfxsdk_base,
            )
            subkeys = []
            i = 0
            while True:
                try:
                    subkeys.append(winreg.EnumKey(key, i))
                    i += 1
                except WindowsError:
                    break
            subkeys.sort(reverse=True)
            key = winreg.OpenKey(
                winreg.HKEY_LOCAL_MACHINE,
                netfxsdk_base + subkeys[0],
            )
        except OSError as e:
            continue
        install_dir, _ = winreg.QueryValueEx(key, "KitsInstallationFolder")
        return Path(install_dir)
    return None

if __name__ == "__main__":
    main()
