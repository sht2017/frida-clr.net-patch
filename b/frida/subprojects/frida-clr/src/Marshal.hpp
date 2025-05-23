#pragma once

#include <frida-core.h>

namespace Frida
{
  private ref class Marshal
  {
  public:
    static System::String ^ UTF8CStringToClrString (const char * str);
    static char * ClrStringToUTF8CString (System::String ^ str);
    static gchar ** ClrStringArrayToUTF8CStringVector (array<System::String ^> ^ arr);
    static array<unsigned char> ^ ByteArrayToClrArray (gconstpointer data, gsize size);
    static array<unsigned char> ^ BytesToClrArray (GBytes * bytes);
    static GBytes * ClrByteArrayToBytes (array<unsigned char> ^ arr);
    static System::Collections::Generic::IDictionary<System::String ^, Object ^> ^ ParametersDictToClrDictionary (GHashTable * dict);
    static Object ^ VariantToClrObject (GVariant * v);
    static array<array<System::Byte>^>^ IconArrayToRgbaArrayArray(Object^ icons);
    static array<System::Byte>^ IconToRgbaArray(Object^ icon);

    static void ThrowGErrorIfSet (GError ** error);
  };
}