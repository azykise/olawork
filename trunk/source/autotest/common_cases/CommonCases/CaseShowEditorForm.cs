using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace common_cases
{
    using editor;
    using Sharp3D.Math.Core;
    using tframework;

    public class CaseShowEditorForm : TestCase
    {
        public CaseShowEditorForm() { }

        public override string name()
        {
            return "CaseShowEditorForm";
        }

        public override int weight()
        {
            return 2;
        }

        public override IEnumerable<TestResult> runSteps()
        {
            mParent.MainForm.AddOwnedForm(CaseGlobal.sEditorForm);
            CaseGlobal.sEditorForm.Show();

            yield return new TestResultYield("show Form Done", 1.0f);

            CaseGlobal.sEditorForm.Hide();

            yield return new TestResultPassed();
        }
    }
}
