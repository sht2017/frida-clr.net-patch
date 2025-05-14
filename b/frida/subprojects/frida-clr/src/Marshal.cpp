#include "Marshal.hpp"

#include <msclr/marshal.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

namespace Frida
{
  String ^
  Marshal::UTF8CStringToClrString (const char * str)
  {
    wchar_t * strUtf16 = reinterpret_cast<wchar_t *> (g_utf8_to_utf16 (str, -1, NULL, NULL, NULL));
    String ^ result = gcnew String (strUtf16);
    g_free (strUtf16);
    return result;
  }

  char *
  Marshal::ClrStringToUTF8CString (String ^ str)
  {
    msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context ();
    const wchar_t * strUtf16 = context->marshal_as<const wchar_t *> (str);
    gchar * strUtf8 = g_utf16_to_utf8 (reinterpret_cast<const gunichar2 *> (strUtf16), -1, NULL, NULL, NULL);
    delete context;
    return strUtf8;
  }

  gchar **
  Marshal::ClrStringArrayToUTF8CStringVector (array<String ^> ^ arr)
  {
    if (arr == nullptr)
      return NULL;
    gchar ** result = g_new0 (gchar *, arr->Length + 1);
    for (int i = 0; i != arr->Length; i++)
      result[i] = Marshal::ClrStringToUTF8CString (arr[i]);
    return result;
  }

  array<unsigned char> ^
  Marshal::ByteArrayToClrArray (gconstpointer data, gsize size)
  {
    if (data == NULL)
      return nullptr;
    array<unsigned char> ^ result = gcnew array<unsigned char> (size);
    pin_ptr<unsigned char> resultStart = &result[0];
    memcpy (resultStart, data, size);
    return result;
  }

  array<unsigned char> ^
  Marshal::BytesToClrArray (GBytes * bytes)
  {
    if (bytes == NULL)
      return nullptr;
    gsize size;
    gconstpointer data = g_bytes_get_data (bytes, &size);
    return ByteArrayToClrArray (data, size);
  }

  GBytes *
  Marshal::ClrByteArrayToBytes (array<unsigned char> ^ arr)
  {
    if (arr == nullptr)
      return NULL;
    pin_ptr<unsigned char> arrStart = &arr[0];
    return g_bytes_new (arrStart, arr->Length);
  }

  IDictionary<String ^, Object ^> ^
  Marshal::ParametersDictToClrDictionary (GHashTable * dict)
  {
    Dictionary<String ^, Object ^> ^ result = gcnew Dictionary<String ^, Object ^> ();

    GHashTableIter iter;
    g_hash_table_iter_init (&iter, dict);

    gpointer rawKey, rawValue;
    while (g_hash_table_iter_next (&iter, &rawKey, &rawValue))
    {
      const gchar * key = static_cast<const gchar *> (rawKey);
      GVariant * value = static_cast<GVariant *> (rawValue);
      result[UTF8CStringToClrString (key)] = VariantToClrObject (value);
    }

    return result;
  }

  Object ^
  Marshal::VariantToClrObject (GVariant * v)
  {
    if (v == NULL)
      return nullptr;

    if (g_variant_is_of_type (v, G_VARIANT_TYPE_STRING))
      return UTF8CStringToClrString (g_variant_get_string (v, NULL));

    if (g_variant_is_of_type (v, G_VARIANT_TYPE_INT64))
      return gcnew Int64 (g_variant_get_int64 (v));

    if (g_variant_is_of_type (v, G_VARIANT_TYPE_BOOLEAN))
      return gcnew Boolean (g_variant_get_boolean (v));

    if (g_variant_is_of_type (v, G_VARIANT_TYPE ("ay")))
    {
      gsize size;
      gconstpointer data = g_variant_get_fixed_array (v, &size, sizeof (guint8));
      return ByteArrayToClrArray (data, size);
    }

    if (g_variant_is_of_type (v, G_VARIANT_TYPE_VARDICT))
    {
      Dictionary<String ^, Object ^> ^ result = gcnew Dictionary<String ^, Object ^> ();

      GVariantIter iter;
      g_variant_iter_init (&iter, v);

      gchar * key;
      GVariant * value;
      while (g_variant_iter_next (&iter, "{sv}", &key, &value))
      {
        result[UTF8CStringToClrString (key)] = VariantToClrObject (value);
        g_variant_unref (value);
        g_free (key);
      }

      return result;
    }

    if (g_variant_is_of_type (v, G_VARIANT_TYPE_ARRAY))
    {
      List<Object ^> ^ result = gcnew List<Object ^> ();

      GVariantIter iter;
      g_variant_iter_init (&iter, v);

      GVariant * value;
      while ((value = g_variant_iter_next_value (&iter)) != NULL)
      {
        result->Add (VariantToClrObject (value));
        g_variant_unref (value);
      }

      return result->ToArray ();
    }

    return nullptr;
  }

  array<array<System::Byte>^>^
      Marshal::IconArrayToRgbaArrayArray(Object^ icons)
  {
      auto result = gcnew List<array<System::Byte>^>();

      auto iconsArray = safe_cast<array<Object^>^> (icons);
      for (int i = 0; i != iconsArray->Length; i++)
      {
          array<System::Byte>^ element = IconToRgbaArray(iconsArray[i]);
          if (element != nullptr)
              result->Add(element);
      }

      return result->ToArray();
  }

  array<System::Byte>^
  Marshal::IconToRgbaArray (Object ^ icon)
  {
    if (icon == nullptr)
      return nullptr;

    array<System::Byte>^ result;

    auto iconDict = safe_cast<IDictionary<String ^, Object ^> ^> (icon);
    String^ format = safe_cast<String^>(iconDict["format"]);
    array<System::Byte>^ image = safe_cast<array<System::Byte>^>(iconDict["image"]);

    if (format == "rgba")
    {
        result = image;
    }
    else if (format == "png")
    {
      MemoryStream^ ms = gcnew MemoryStream(image);
      Bitmap^ bmp = gcnew Bitmap(ms);

      int width = bmp->Width;
      int height = bmp->Height;
      System::Drawing::Rectangle rect(0, 0, width, height);
      BitmapData^ bd = bmp->LockBits(rect, ImageLockMode::ReadOnly, PixelFormat::Format32bppArgb);

      try
      {
        int stride = bd->Stride;
        int totalBytes = Math::Abs(stride) * height;
        array<Byte>^ rawData = gcnew array<Byte>(totalBytes);

        System::Runtime::InteropServices::Marshal::Copy(bd->Scan0, rawData, 0, totalBytes);

        for (int i = 0; i < totalBytes; i += 4)
        {
            Byte b = rawData[i + 0];
            Byte g = rawData[i + 1];
            Byte r = rawData[i + 2];
            Byte a = rawData[i + 3];
            rawData[i + 0] = r;
            rawData[i + 1] = g;
            rawData[i + 2] = b;
            rawData[i + 3] = a;
        }

        result = rawData;
      }
      finally
      {
          bmp->UnlockBits(bd);
          delete bmp;
          delete ms;
      }
    }
    else
    {
       result = nullptr;
    }

    return result;
  }

  void
  Marshal::ThrowGErrorIfSet (GError ** error)
  {
    if (*error == NULL)
      return;
    String ^ message = UTF8CStringToClrString ((*error)->message);
    g_clear_error (error);
    throw gcnew Exception (message);
  }
}
