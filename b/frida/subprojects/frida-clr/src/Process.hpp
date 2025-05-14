#pragma once

#include <frida-core.h>

using namespace System;
using namespace System::Collections::Generic;

namespace Frida
{
  public ref class Process
  {
  internal:
    Process (FridaProcess * handle);
  public:
    ~Process ();
  protected:
    !Process ();

  public:
    property unsigned int Pid { unsigned int get (); }
    property String ^ Name { String ^ get (); }
    property IDictionary<String ^, Object ^> ^ Parameters { IDictionary<String ^, Object ^> ^ get (); }
    property array<array<System::Byte>^> ^ IconDatas { array<array<System::Byte>^> ^ get (); }

    virtual String ^ ToString () override;

  private:
    FridaProcess * handle;

    IDictionary<String ^, Object ^> ^ parameters;
    array<array<System::Byte>^> ^ iconDatas;
  };
}