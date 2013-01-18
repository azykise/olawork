using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;
using tframework;

namespace common_cases
{
    using editor;
    using Sharp3D.Math.Core;

    public class CaseSphereSpace : TestCase
    {
        SphereSpace sp = new SphereSpace();

        public CaseSphereSpace()
        {
            sp.radius = 1.0f;
            sp.degreeZ = 90.0f;
            sp.degreeY = 0.0f;
        }

        public override string name()
        {
            return "CaseSphereSpace";
        }

        public override IEnumerable<TestResult> runSteps()
        {
            sp.radius = 1.0f;
            sp.degreeZ = 90.0f;
            sp.degreeY = 0.0f;

            Vector3F v = sp.orthPosition();

            if (MathFunctions.Abs(v.Y - 1.0f) <= MathFunctions.EpsilonF &&
                 MathFunctions.Abs(v.Z) <= MathFunctions.EpsilonF &&
                MathFunctions.Abs(v.X) <= MathFunctions.EpsilonF)
            {
                yield return new TestResultYield("passed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString(), 0.1f);
            }
            else
                yield return new TestResultFailed("failed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString());


            sp.radius = 1.0f;
            sp.degreeZ = 90.0f;
            sp.degreeY = 90.0f;

            v = sp.orthPosition();

            if (MathFunctions.Abs(v.Y) <= MathFunctions.EpsilonF &&
                 MathFunctions.Abs(v.Z) <= MathFunctions.EpsilonF &&
                MathFunctions.Abs(v.X - 1.0f) <= MathFunctions.EpsilonF)
            {
                yield return new TestResultYield("passed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString(), 0.1f);
            }
            else
                yield return new TestResultFailed("failed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString());


            sp.radius = 1.0f;
            sp.degreeZ = 90.0f;
            sp.degreeY = 180.0f;

            v = sp.orthPosition();

            if (MathFunctions.Abs(v.Y + 1.0f) <= MathFunctions.EpsilonF &&
                 MathFunctions.Abs(v.Z) <= MathFunctions.EpsilonF &&
                MathFunctions.Abs(v.X) <= MathFunctions.EpsilonF)
            {
                yield return new TestResultYield("passed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString(), 0.1f);
            }
            else
                yield return new TestResultFailed("failed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString());


            sp.radius = 1.0f;
            sp.degreeZ = 90.0f;
            sp.degreeY = 270.0f;

            v = sp.orthPosition();

            if (MathFunctions.Abs(v.Y) <= MathFunctions.EpsilonF &&
                 MathFunctions.Abs(v.Z) <= MathFunctions.EpsilonF &&
                MathFunctions.Abs(v.X + 1.0f) <= MathFunctions.EpsilonF)
            {
                yield return new TestResultYield("passed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString(), 0.1f);
            }
            else
                yield return new TestResultFailed("failed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString());

            sp.radius = 1.0f;
            sp.degreeZ = 90.0f;
            sp.degreeY = 360.0f;

            v = sp.orthPosition();

            if (MathFunctions.Abs(v.Y - 1.0f) <= MathFunctions.EpsilonF &&
                 MathFunctions.Abs(v.Z) <= MathFunctions.EpsilonF &&
                MathFunctions.Abs(v.X) <= MathFunctions.EpsilonF)
            {
                yield return new TestResultYield("passed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString(), 0.1f);
            }
            else
                yield return new TestResultFailed("failed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString());


            sp.radius = 1.0f;
            sp.degreeZ = 180.0f;
            sp.degreeY = 0.0f;

            v = sp.orthPosition();

            if (MathFunctions.Abs(v.Y) <= MathFunctions.EpsilonF &&
                 MathFunctions.Abs(v.Z + 1.0f) <= MathFunctions.EpsilonF &&
                MathFunctions.Abs(v.X) <= MathFunctions.EpsilonF)
            {
                yield return new TestResultYield("passed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString(), 0.1f);
            }
            else
                yield return new TestResultFailed("failed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString());


            sp.radius = 1.0f;
            sp.degreeZ = 270.0f;
            sp.degreeY = 0.0f;

            v = sp.orthPosition();

            if (MathFunctions.Abs(v.Y + 1.0f) <= MathFunctions.EpsilonF &&
                 MathFunctions.Abs(v.Z) <= MathFunctions.EpsilonF &&
                MathFunctions.Abs(v.X) <= MathFunctions.EpsilonF)
            {
                yield return new TestResultYield("passed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString(), 0.1f);
            }
            else
                yield return new TestResultFailed("failed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString());

            sp.radius = 1.0f;
            sp.degreeZ = 270.0f;
            sp.degreeY = -90.0f;

            v = sp.orthPosition();

            if (MathFunctions.Abs(v.Y) <= MathFunctions.EpsilonF &&
                 MathFunctions.Abs(v.Z) <= MathFunctions.EpsilonF &&
                MathFunctions.Abs(v.X - 1.0f) <= MathFunctions.EpsilonF)
            {
                yield return new TestResultYield("passed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString(), 0.1f);
            }
            else
                yield return new TestResultFailed("failed dz=" + sp.degreeZ.ToString() + " dy=" + sp.degreeY.ToString());

            sp = new SphereSpace();
            sp.fromOrth(new Vector3F(200.0f,200.0f,200.0f));
            float dy = sp.degreeY;
            float cy = dy + 10.0f;
            while (dy < cy)
            {
                sp.degreeY = dy;

                Vector3F p = sp.orthPosition();

                yield return new TestResultYield("Orth Pos:" + p.ToString(), 0.1f);

                dy++;
            }

            yield return new TestResultPassed();
        }
    }
}
