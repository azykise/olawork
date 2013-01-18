using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace editor
{
    public interface IRenderPanel
    {
        IntPtr getPanelHandle();
        int getPanelWidth();
        int getPanelHeight();
    }
}
