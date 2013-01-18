using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;
namespace modelview
{
    public class ModelView : Domain
    {
        public ModelView()
            : base(ModelViewName,"模型查看器")
        {
            m_modelview_form = new ModelView_Form();
            this.m_main_form = m_modelview_form;

            registMessage("ExpandRightPanel", OnMessageExpandRightPanel);
        }

        public override void init(Engine.RenderSystem rsys)
        {
            base.init(rsys);

            ModelViewDoc mdoc = new ModelViewDoc(this);
            m_current_doc = mdoc;

            m_current_doc.newDoc();

            MainView main_v = new MainView(rsys, mdoc);
            m_view_mng.addView(main_v);
            m_view_mng.CurrentView = main_v;
            
            m_view_mng.CurrentView.RenderWindow = rsys.createWindow(m_modelview_form.RenderPanel.Width,
                m_modelview_form.RenderPanel.Height, m_modelview_form.RenderPanel.Handle);

            m_view_mng.CurrentView.Active = true;

            m_modelview_form.RenderPanel.setCurrentView(main_v);

            EditorGraph.registerComponent(this, main_v);
            EditorGraph.registerComponent(this, m_current_doc);

            editor.ui.ToolBarDocControl toolbardoc = new editor.ui.ToolBarDocControl(this.getName());
            m_modelview_form.mergeToolBar(toolbardoc.ToolBar);
            EditorGraph.registerComponent(this, toolbardoc.EditorComponent);

            ToolBarCommonComponent toolbarcommon_comp = new ToolBarCommonComponent(mdoc);
            m_modelview_form.mergeToolBar(toolbarcommon_comp.ToolbarUI.ToolBar);
            EditorGraph.registerComponent(this, toolbarcommon_comp);

            ActionTimeLineComponent actiontimeline_comp = new ActionTimeLineComponent(mdoc);
            m_modelview_form.insertDockControl(ModelView_Form.DOCK_TYPE_BUTTOM, "",actiontimeline_comp.ActionTimeLineUI);
            EditorGraph.registerComponent(this, actiontimeline_comp);

            BoneListComponent bonelist_comp = new BoneListComponent(mdoc);
            m_modelview_form.insertDockControl(ModelView_Form.DOCK_TYPE_RIGHT_TAB1,"骨架",bonelist_comp.BoneListUI);
            EditorGraph.registerComponent(this, bonelist_comp);

            ActionListComponent actionlist_comp = new ActionListComponent(mdoc);
            m_modelview_form.insertDockControl(ModelView_Form.DOCK_TYPE_RIGHT_TAB2, "动作", actionlist_comp.ActionListUI);
            EditorGraph.registerComponent(this, actionlist_comp);

            m_modelview_form.setRightPanelExpand(false);

            EditorGraph.postMessage(this, new MessageBase("AfterModelViewUICreated"));
        }

        private void OnMessageExpandRightPanel(EditorComponent from, MessageBase msg)
        {
            if (msg.Messages.Count != 0)
            {
                string info0 = msg.Messages[0];
                m_modelview_form.setRightPanelExpand(info0 == "true");
            }
        }

        protected ModelView_Form m_modelview_form = null;
        public const string ModelViewName = "ModelView";
    }
}
