using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;

namespace scene
{
    public class SceneEditor : Domain 
    {
        public SceneEditor()
            :base("SceneEditor","场景编辑器")
        {
            m_scene_form = new SceneEditor_Form();
            this.m_main_form = m_scene_form;
        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            NotifyResult nr = base.notified(from, msg);

            if (msg.MsgType == MessageType.NewDoc)
            {
                m_current_doc.newDoc();
            }

            return nr;
        }

        public override void init(Engine.RenderSystem rsys)
        {
            base.init(rsys);

            m_current_doc = new SceneEditorDoc(0);
            EditorGraph.registerComponent(this, m_current_doc);

            SceneEntityManager entity_mng = new SceneEntityManager();
            m_current_doc.EntityMng = entity_mng;
            EditorGraph.registerComponent(this, entity_mng);

            MainView main_v = new MainView(rsys);
            m_view_mng.addView(main_v);
            m_view_mng.CurrentView = main_v;           
            m_view_mng.CurrentView.RenderWindow = rsys.createWindow(m_scene_form.RenderPanel.Width,
                m_scene_form.RenderPanel.Height, m_scene_form.RenderPanel.Handle);
            m_view_mng.CurrentView.Active = true;
            m_scene_form.RenderPanel.setCurrentView(main_v);
            EditorGraph.registerComponent(this, main_v);

            editor.ui.ToolBarDocControl toolbardoc = new editor.ui.ToolBarDocControl(this.getName());
            m_scene_form.mergeToolBar(toolbardoc.ToolBar);
            EditorGraph.registerComponent(this, toolbardoc.EditorComponent);

            editor.ui.ToolBar3DControl toolbar3d = new editor.ui.ToolBar3DControl(this.getName());
            m_scene_form.mergeToolBar(toolbar3d.ToolBar);
            EditorGraph.registerComponent(this, toolbar3d.EditorComponent);

            EntityCreate panel_create = new EntityCreate();
            m_scene_form.insertDockControl(new SceneEditorDockTo(SceneEditor_FormDockID.CreateDock), panel_create);
            EditorGraph.registerComponent(this, panel_create.EditorComponent);

            //editor.ui.CommonPropertyGrid property_grid = new editor.ui.CommonPropertyGrid();
            //m_scene_form.insertDockControl(new SceneEditorDockTo(SceneEditor_FormDockID.PropertyDock), property_grid);

            EntityPropertyGrid property_grid = new EntityPropertyGrid();
            m_scene_form.insertDockControl(new SceneEditorDockTo(SceneEditor_FormDockID.PropertyDock), property_grid);
            EditorGraph.registerComponent(this, property_grid.EditorComponent);

            EntityList panel_list = new EntityList();
            m_scene_form.insertDockControl(new SceneEditorDockTo(SceneEditor_FormDockID.ListDock), panel_list);
            EditorGraph.registerComponent(this, panel_list.EditorComponent);
        }

        SceneEditor_Form m_scene_form = null;
    }
  
}
