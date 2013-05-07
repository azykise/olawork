using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using Sharp3D.Math.Core;

namespace editor
{
	public class Primitive
	{
		public Primitive(IntPtr p,RenderSystem rendersys)
        {

        }

		public  void render(RenderSystem rendersys)
        {

        }
		
		public void setColor(float r,float g,float b,float a)
        {

        }

		//IPrimitive* getPrimitive()	{return m_primitive;} 

		public static float computeDottedSpace(float line_length)
        {
            return 0;
        }

		public UInt64 getID(){return m_id;}
	
		public RenderSystem getRenderSystem() {return m_rendersys;}

	
		protected RenderSystem m_rendersys;
		protected UInt64 m_id;
        protected IntPtr m_primitive;
	};

    public class MathHelper
    {
        public static bool IsZero(float f)
        {
            return f >= -1.0f * float.Epsilon && f <= float.Epsilon;
        }
    }

    public class PrimitiveBorderCube : Primitive
    {
		public PrimitiveBorderCube(IntPtr p,RenderSystem rendersys):
            base(p,rendersys)
        {

        }
    }

    public class PrimitiveGrid : Primitive
    {
        public PrimitiveGrid(IntPtr p, RenderSystem rendersys) :
            base(p,rendersys)
        {

        }
    }

    public class PrimitivePlane : Primitive
    {
        public PrimitivePlane(IntPtr p, RenderSystem rendersys):
            base(p, rendersys)
        {

        }
    }

	public class RenderWindow
	{
        public RenderWindow(System.IntPtr wnd)
        {
            mWindowPtr = wnd;
        }

        public void resetWindow(int w, int h, System.IntPtr hwnd)
        {

        }

        public void setBackgroundColor(float r, float g, float b, float a)
        {

        }

		public int getWidth()
        {
            return 0;
        }

		public int getHeight() 
        {
            return 0;
        }
	 
		public System.IntPtr  mWindowPtr;
	};

	public class ViewParament
	{
	    public ViewParament()
		{
			Position[0] = Position[1] = Position[2] = 0.0f;
			LookAtPt[0] = LookAtPt[1] = LookAtPt[2] = 0.0f;

			Aspect = 4.0f/3.0f;
			Fovy = (float)(System.Math.PI/4);
			NearDist = 1.0f;
			FarDist = 1001.0f;
		}
        public float[] Position = new float[3];
        public float[] LookAtPt = new float[3];
        public float Fovy;
        public float Aspect;
        public float NearDist;
        public float FarDist;	
	};

    public class RenderSystem
    {
        public RenderSystem(IntPtr hwnd)
        {
            mRenderCore = EngineWrapper.createRenderCore(hwnd);
        }

        public void release()
        {
            if (mRenderCore != IntPtr.Zero)
            {
                EngineWrapper.deleteRenderCore(mRenderCore);
            }
        }

		public RenderWindow createWindow(int w,int h,IntPtr hwnd)
        {
            IntPtr pWnd = EngineWrapper.RenderCore_CreateWindow(mRenderCore, hwnd, w, h);
            RenderWindow wnd = new RenderWindow(pWnd);
            return wnd;
        }

		public void setRenderWindow(RenderWindow wnd)
        {
            EngineWrapper.RenderCore_SetWindow(mRenderCore, wnd.mWindowPtr);
        }

        public void setViewParament(ViewParament param)
        {
            EngineWrapper.RenderCore_SetView(mRenderCore,
                param.Position[0], param.Position[1], param.Position[2],
                param.LookAtPt[0], param.LookAtPt[2], param.LookAtPt[2],
                param.Fovy, param.Aspect, param.NearDist, param.FarDist);
        }

        public Matrix4F getViewMatrix()
        {
            IntPtr f44 = EngineWrapper.RenderCore_MallocFloats(16);
            EngineWrapper.RenderCore_GetViewMatrix(mRenderCore, f44);

            float[] fs = new float[16];
            Marshal.Copy(f44, fs, 0, 16);

            EngineWrapper.RenderCore_FreeFloats(f44);

            Matrix4F m44 = new Matrix4F(fs);
            return m44;
        }

        public Matrix4F getProjMatrix()
        {
            IntPtr f44 = EngineWrapper.RenderCore_MallocFloats(16);
            EngineWrapper.RenderCore_GetProjMatrix(mRenderCore, f44);

            float[] fs = new float[16];
            Marshal.Copy(f44, fs, 0, 16);

            EngineWrapper.RenderCore_FreeFloats(f44);

            Matrix4F m44 = new Matrix4F(fs);
            return m44;
        }

        public void beginDraw()
        {
            EngineWrapper.RenderCore_BeginDraw(mRenderCore);
        }

        public void drawSceneActors()
        {
            EngineWrapper.RenderCore_DrawSceneActors(mRenderCore);
        }

        public void endDraw()
        {
            EngineWrapper.RenderCore_EndDraw(mRenderCore);
        }

		public void beginHardwareSelect(int x,int y,int w,int h)
        {

        }

		public int  endHardwareSelect()
        {
            return 0;
        }

		public uint[] getSelectResult(int count)
        {
            uint[] r = new uint[0];
            return r;
        }

        protected IntPtr mRenderCore = IntPtr.Zero;
    }

    public class EngineWrapper
    {
        [DllImport("ola_render.dll")]
        public static extern IntPtr createRenderCore(IntPtr hwnd);

        [DllImport("ola_render.dll")]
        public static extern void deleteRenderCore(IntPtr core);

        [DllImport("ola_render.dll")]
        public static extern IntPtr RenderCore_MallocFloats(uint size);

        [DllImport("ola_render.dll")]
        public static extern IntPtr RenderCore_MallocInts(uint size);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_FreeFloats(IntPtr fs);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_FreeInts(IntPtr ins);

        [DllImport("ola_render.dll")]
        public static extern uint RenderCore_Initialize(IntPtr core,IntPtr hwnd);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_Release(IntPtr core);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_BeginDraw(IntPtr core);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_EndDraw(IntPtr core);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_ReleaseObject(IntPtr core, IntPtr objs);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_DrawSceneActors(IntPtr core);

        [DllImport("ola_render.dll")]
        public static extern IntPtr RenderCore_CreateWindow(
            IntPtr core, IntPtr hwnd, int w, int h);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_SetWindow(IntPtr core, IntPtr wnd);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_SetView(IntPtr core,
            float px, float py, float pz,
            float lx, float ly, float lz,
            float fovy, float aspect, float n, float f);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_GetView(IntPtr core,
            ref float px, ref float py, ref float pz,
            ref float lx, ref float ly, ref float lz,
            ref float fovy, ref float aspect, ref float n, ref float f);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_GetViewMatrix(IntPtr core, IntPtr f44);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_GetProjMatrix(IntPtr core, IntPtr f44);

        [DllImport("ola_render.dll")]
        public static extern IntPtr RenderCore_CreatePrimitive(IntPtr core);

        [DllImport("ola_render.dll")]
        public static extern void RenderCore_PushPrimitive(IntPtr core, IntPtr p);

        [DllImport("ola_render.dll")]
        public static extern void Primitive_SetVertex(IntPtr prim, IntPtr vec3s, int length);

        [DllImport("ola_render.dll")]
        public static extern void Primitive_SetIndex(IntPtr prim, IntPtr idxs, int length);
    }
}
