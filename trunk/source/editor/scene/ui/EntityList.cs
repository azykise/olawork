using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using editor;

namespace scene
{

    partial class EntityList : UserControl
    {        
        public EntityList()
        {
            InitializeComponent();
            mComponent = new EntityListComponent(this);
        }

        public EntityListComponent EditorComponent { get { return mComponent; } }
        EntityListComponent mComponent = null;
        SceneEntityManager mEntityMng = null;


        public void initialize()
        {
            //this.advTreeLayer.Nodes.Clear();
            //this.advTreeEntity.Nodes.Clear();

            //Domain domain = DomainManager.Instance().getDomain(mComponent);
            //EditorDoc doc = domain.getCurrentDoc();
            //if (doc == null)
            //    return;

            //mEntityMng = doc.EntityMng as SceneEntityManager;

            //refreshLayerTree(mEntityMng);
        }

        public void refreshLayerTree(EntityManager entity_mng)
        {
            //this.advTreeLayer.Nodes.Clear();
            //this.advTreeEntity.Nodes.Clear();

            //DevComponents.AdvTree.Node layer_root = new DevComponents.AdvTree.Node();
            //layer_root.Text = "物品层";
            //layer_root.Name = "LayerRoot";
            //layer_root.DataKey = null;
            //this.advTreeLayer.Nodes.Add(layer_root);

            //DevComponents.AdvTree.Node current_layer_node = null;
            //foreach (EntityLayer layer in entity_mng.getEntityLayers())
            //{
            //    DevComponents.AdvTree.Node node = new DevComponents.AdvTree.Node();
            //    node.Text = layer.Name;
            //    node.DataKey = layer;
            //    layer_root.Nodes.Add(node);

            //    if (entity_mng.CurrentEntityLayer != null && entity_mng.CurrentEntityLayer.Name == layer.Name)
            //    {
            //        current_layer_node = node;
            //    }
            //}

            //if (current_layer_node != null)
            //{
            //    this.advTreeLayer.SelectNode(current_layer_node, DevComponents.AdvTree.eTreeAction.Code);
            //    current_layer_node.Expanded = true;
            //}

            //this.advTreeLayer.Refresh();   
        }

        public void refreshEntityTree(EntityLayer layer)
        {
            //this.advTreeEntity.Nodes.Clear();

            //if (layer == null)
            //{
            //    if (advTreeLayer.SelectedNode != null)
            //    {
            //        layer = this.advTreeLayer.SelectedNode.DataKey as EntityLayer;
            //        if (layer == null)
            //        {
            //            return;
            //        }
            //    }
            //}

            //Node entity_root = new Node();
            //entity_root.Text = "物品列表";
            //entity_root.Name = "EntityRoot";
            //entity_root.DataKey = null;
            //this.advTreeEntity.Nodes.Add(entity_root);

            //foreach (Entity entity in layer.OriginBuffer)
            //{
            //    DevComponents.AdvTree.Node node = new DevComponents.AdvTree.Node();
            //    node.Text = entity.Name;
            //    node.DataKey = entity;
            //    entity_root.Nodes.Add(node);
            //}

            //entity_root.Expanded = true;
        }

        //private void advTreeLayer_AfterNodeSelect(object sender, DevComponents.AdvTree.AdvTreeNodeEventArgs e)
        //{
        //    DevComponents.AdvTree.Node sel_node = e.Node;
        //    if (sel_node == null)
        //    {
        //        return;
        //    }

        //    EntityLayer layer = sel_node.DataKey as EntityLayer;
        //    refreshEntityTree(layer);
        //}

        //private void advTreeEntity_AfterNodeSelect(object sender, AdvTreeNodeEventArgs e)
        //{
        //    DevComponents.AdvTree.Node sel_node = e.Node;
        //    if (sel_node != null)
        //    {
        //        Entity entity = sel_node.DataKey as Entity;
        //        if (entity != null && entity.Layer != null)
        //        {
        //            entity.Layer.SelectBuffer.clear();
        //            entity.Layer.SelectBuffer.insert(entity);
        //        }
        //        else
        //            mEntityMng.CurrentEntityLayer.SelectBuffer.clear();
        //    }
        //    else
        //    {
        //        foreach (EntityLayer layer in mEntityMng.getEntityLayers())
        //        {
        //            layer.SelectBuffer.clear();
        //        }
        //    }

        //    Domain domain = DomainManager.Instance().getDomain(mComponent);
        //    EditorGraph.broadCast(domain, new CastMessage(MessageType.Editor, CommonEntityNode.SelectedEntityChanged));
        //}

        private void expandPanel1_OnExpandChange(bool expand)
        {
            this.splitContainer1.Panel1Collapsed = !expand;
        }
    }
}
