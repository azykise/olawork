using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using tframework;
using editor;
using Sharp3D.Math.Core;
using System.Windows.Forms;
using System.Reflection;

namespace common_cases
{
    public class CaseLightingModel : TestCase
    {
        public CaseLightingModel() { }

        public override string name()
        {
            return "CaseLightingModel";
        }

        public override void kill()
        {
            CommonCaseTool.modelviewCloseModel();
        }

        public override IEnumerable<TestResult> runSteps()
        {
            if (!mParent.MainForm.Contains(CaseGlobal.sEditorForm))
                mParent.MainForm.AddOwnedForm(CaseGlobal.sEditorForm);

            CaseGlobal.sEditorForm.Show();

            yield return new TestResultYield("Show Form Done", 1.0f);

            string file_name = editor.Tool.GetAssetFolderFullPath() + "art\\common\\waitingroom001_teapoy001.dml";

            string error = CommonCaseTool.modelviewLoadModel(file_name);
            if (error.Length == 0)
                yield return new TestResultYield("open File Done", 1.0f);
            else
                yield return new TestResultFailed(error);

            yield return new TestResultYield("Wait For 1 sec", 1.0f);


            editor.SphereSpace sp = new editor.SphereSpace();
            sp.fromOrth(new Vector3F(200.0f, 200.0f, 200.0f));

            float dy = sp.degreeY;

            EditorDoc doc = CaseGlobal.sEditorForm.DomainMng.MainDomain.getCurrentDoc();
            editor.View view = CaseGlobal.sEditorForm.DomainMng.MainDomain.getViewManager().CurrentView;
            editor.ComplexController base_ctrl = view.BaseController;

            base_ctrl.handleKeyboardEvent(new KeyboardInputEvent(new KeyEventArgs(Keys.L),InputAction.OnKeyPressing));

            float cy = 720.0f + dy;

            while (dy <= cy)
            {
                sp.degreeY = dy;

                Vector3F p = sp.orthPosition();                

                Controller c = base_ctrl.CurrentSubController;

                Type t = c.GetType();
                MethodInfo mi = t.GetMethod("moveingLight");
                object[] parmArray = new object[] { p, new Vector3F(0, 0, 0) };
                mi.Invoke(c, parmArray);

                dy += 3.0f;

                yield return new TestResultYield("LigjtSrc Pos: " + p.ToString(), 0.01f);
            }

            base_ctrl.handleKeyboardEvent(new KeyboardInputEvent(new KeyEventArgs(Keys.L), InputAction.OnKeyUp));

            error = CommonCaseTool.modelviewCloseModel();
            if (error.Length == 0)
                yield return new TestResultYield("Close File Done", 1.0f);
            else
                yield return new TestResultFailed(error);

            CaseGlobal.sEditorForm.Hide();
            yield return new TestResultPassed();
        }
    }
}
