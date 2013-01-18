using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using tframework;
using editor;
using System.Windows.Forms;

namespace common_cases
{
    public class CaseOpenModel : TestCase
    {
        public CaseOpenModel() { }

        public override string name()
        {
            return "CaseOpenModel";
        }

        public override IEnumerable<TestResult> runSteps()
        {
            if(!mParent.MainForm.Contains(CaseGlobal.sEditorForm))
                mParent.MainForm.AddOwnedForm(CaseGlobal.sEditorForm);

            CaseGlobal.sEditorForm.Show();

            yield return new TestResultYield("Show Form Done", 1.0f);

            string file_name = editor.Tool.GetAssetFolderFullPath() + "art\\common\\waitingroom001_teapoy001.dml";

            for (int i = 0 ; i < 10 ; i++ )
            {
                string error = CommonCaseTool.modelviewLoadModel(file_name);
                if (error.Length == 0)
                    yield return new TestResultYield("open File Done", 1.0f);
                else
                    yield return new TestResultFailed(error);

                yield return new TestResultYield("Wait For 3 sec", 3.0f);

                error = CommonCaseTool.modelviewCloseModel();
                if (error.Length == 0)
                    yield return new TestResultYield("Close File Done", 1.0f);
                else
                    yield return new TestResultFailed(error);

                Engine.RenderSystem rendersys = CaseGlobal.sEditorForm.DomainMng.MainDomain.getViewManager().RenderSystem;
                int res_num = rendersys.getResourceNum("");

                if (res_num == 0)
                    yield return new TestResultYield("Clear Cache Done", 1.0f);
                else
                    yield return new TestResultFailed("Clear Cache Failed !");
            }
            
            CaseGlobal.sEditorForm.Hide();

            yield return new TestResultPassed();
        }
    }
}
