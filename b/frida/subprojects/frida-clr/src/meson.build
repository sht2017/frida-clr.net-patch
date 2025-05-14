sources = [
  'DeviceManager.cpp',
  'Device.cpp',
  'Process.cpp',
  'Session.cpp',
  'Script.cpp',
  'Runtime.cpp',
  'Marshal.cpp',
]

shared_module('Frida', sources,
  override_options: 'cpp_eh=a',
  cpp_args: [
    '/clr',
    '/permissive-',
    '/AI' + dotnet_sdk,
    '/FU' + (dotnet_runtime / 'System.Runtime.dll'),
    '/AI' + dotnet_desktop_runtime,
    '/FU' + (dotnet_desktop_runtime / 'System.Drawing.Common.dll'),
    '/FU' + (dotnet_runtime / 'System.Drawing.Primitives.dll'),
  ],
  link_args: [
    '/IGNORE:4248',
    '/LIBPATH:' + (netfx_sdk / 'lib' / 'um' / host_msvc_platform),
  ],
  dependencies: frida_core_dep,
  install: true,
  install_dir: get_option('bindir'),
)
