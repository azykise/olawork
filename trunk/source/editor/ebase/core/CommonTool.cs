using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace editor
{
    public class CommonTool
    {
        public static void newLog(EditorComponent from,string msg)
        {
            EditorGraph.notify("Log", from, new MessageBase(MessageType.Log, from.getName(), msg));
        }
    }
}
