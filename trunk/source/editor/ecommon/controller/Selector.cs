using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace editor
{
    public class Selector
    {
        public static int DownSelectWidth = 5;
        public static int DownSelectHeight = 5;

        public static bool doDownSelect(Point screenpt,View view)
        {
            return doHardwareSelect(screenpt, new Point(screenpt.X - DownSelectWidth, screenpt.Y + DownSelectHeight), view);
        }

        public static bool doDragSelect(Point screen_start,Point screen_end,View view)
        {
            return doHardwareSelect(screen_start,screen_end,view);
        }

        public static bool doHardwareSelect(Point screen_start, Point screen_end, View view)
        {

            Domain domain = DomainManager.Instance().getDomain(view);
            if (domain != null)
            {
                EntityManager mng = domain.getCurrentDoc().EntityMng;
                Dictionary<uint, Dictionary<EntityLayer, Entity>> selectid_cache = new Dictionary<uint, Dictionary<EntityLayer, Entity>>();

                uint id = 1;
                foreach (EntityLayer layer in mng.getEntityLayers())
                {
                    foreach (Entity entity in layer.OriginBuffer)
                    {
                        if (entity.pushToSelect(id, view.RenderSystem))
                        {
                            Dictionary<EntityLayer, Entity> pair = new Dictionary<EntityLayer, Entity>();
                            pair[layer] = entity;
                            selectid_cache[id] = pair;
                            id++;
                        }
                    }
                }

                Rectangle sel_rect = Tool.MakeScreenRect(screen_start, screen_end);
                view.RenderSystem.beginHardwareSelect(sel_rect.X, sel_rect.Y, sel_rect.Width, sel_rect.Height);
                view.RenderSystem.drawSceneActors();
                int count = view.RenderSystem.endHardwareSelect();
                uint[] res = view.RenderSystem.getSelectResult(count);

                foreach (Dictionary<EntityLayer, Entity> pair in selectid_cache.Values)
                {
                    foreach (EntityLayer layer in pair.Keys)
                        layer.SelectBuffer.clear();
                }

                foreach (uint _id in res)
                {
                    if (selectid_cache.ContainsKey(_id))
                    {
                        Dictionary<EntityLayer, Entity> pair = selectid_cache[_id];
                        foreach (KeyValuePair<EntityLayer, Entity> kvp in pair)
                        {
                            kvp.Key.SelectBuffer.insert(kvp.Value);
                        }
                    }
                }

                EditorGraph.broadCast(domain, domain, new MessageBase(MessageType.Editor, CommonEntityNode.SelectedEntityChanged));

                return res.Length > 0;
            }
            else
                return false;
        }

        public enum SelectedTransType : uint
        {
            SELECT_NONE = 0,
            SELECT_MOVE_X,
            SELECT_MOVE_Y,
            SELECT_MOVE_Z,
            SELECT_MOVE_XY,
            SELECT_MOVE_XZ,
            SELECT_MOVE_YZ,
            SELECT_ROTATE_X,
            SELECT_ROTATE_Y,
            SELECT_ROTATE_Z,
            SELECT_SCALE_X,
            SELECT_SCALE_Y,
            SELECT_SCALE_Z,
            SELECT_SCALE_XYZ,
        }

        public static int TransSelectWidth = 5;
        public static int TransSelectHeight = 5;

        public static SelectedTransType selectTransformer(Point screenpt, List<EntityTransform3D> trans, View view)
        {
            Point point_start = screenpt;
            Point point_end = new Point(screenpt.X - TransSelectWidth, screenpt.Y + TransSelectHeight);



            return SelectedTransType.SELECT_NONE;
        }

    }
}
