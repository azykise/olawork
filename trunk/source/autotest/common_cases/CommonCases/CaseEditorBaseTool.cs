using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using tframework;
using editor;
using System.IO;

namespace common_cases
{
    public class CaseEditorBaseTool : TestCase
    {
        public CaseEditorBaseTool() { }

        public override string name()
        {
            return "CaseEditorBaseTool";
        }

        public override int weight() { return 1000; }

        public override IEnumerable<TestResult> runSteps()
        {
            string s = editor.Tool.GetAssetFolderFullPath();

            DirectoryInfo a_folder = new DirectoryInfo(s + "art");
            DirectoryInfo d_folder = new DirectoryInfo(s + "data");
            DirectoryInfo s_folder = new DirectoryInfo(s + "shaders");

            if (a_folder.Exists && d_folder.Exists && s_folder.Exists)
                yield return new TestResultYield("AssetFolderFullPath: " + s + " Done!", 2.0f);
            else
                yield return new TestResultFailed("AssetFolderFullPath Failed!");

            yield return new TestResultPassed();
        }
    }
}
