using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using tframework;
using editor;

namespace common_cases
{
    class CommonCaseTool
    {
        public static string modelviewLoadModel(string file_name)
        {
            EditorDoc doc = CaseGlobal.sEditorForm.DomainMng.MainDomain.getCurrentDoc();
            string error = "";
            try
            {
                error = doc.open(file_name) ? "" : "open File " + file_name + " failed!";
            }
            catch (System.Exception ex)
            {
                error = ex.StackTrace;
            }

            return error;
        }


        public static string modelviewCloseModel()
        {
            EditorDoc doc = CaseGlobal.sEditorForm.DomainMng.MainDomain.getCurrentDoc();
            string error = "";
            try
            {
                doc.close();
            }
            catch (System.Exception ex)
            {
                error = ex.StackTrace;
            }
            return error;
        }
    }
}
