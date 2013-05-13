global g_mmdlMainFloater = undefined
global g_mmdlMenu = undefined

global g_mmdlConfig = undefined

global g_mmdlMenuName = "Mmdl"
global g_editorExeName = "editor.exe"
global g_editorHWND = undefined
global MSG_ID_MMDL  = 0x0054 + 131

struct MmdlConfig
(	
public	
	mVersion = "1.00",	
	mEditorFullname = "",	
	mIniFileFullname = (getDir #maxRoot) + "\\" + "MmdlScriptConfig.ini",
	
	function checkIniFile = 
	(
		ini_exist = doesFileExist mIniFileFullname 
		if(ini_exist == false) then
		(
			setINISetting mIniFileFullname "Version" "v" mVersion 
			print("create mmdl ini file...")
		)
		else
		(
			_version = getINISetting mIniFileFullname "version" "v"
			print("mmdl ini file existed version = " + _version)
		)
	),
	
private
	SECTION_DIRECTIONS = "Directions",
		KEY_EDITOR_FULLNAME = "EditorFullname",
	
public
	function readConfig = 
	(
		mEditorFullname = getINISetting mIniFileFullname SECTION_DIRECTIONS KEY_EDITOR_FULLNAME		
	),
	
	function saveConfig = 
	(
		setINISetting mIniFileFullname SECTION_DIRECTIONS KEY_EDITOR_FULLNAME mEditorFullname
	),

	on create do(
		checkIniFile()
	)	
)

struct MmdlWinMsg
(
private	
	MSG_ID_START = 0,
public
	MSG_ID_REFRESH = MSG_ID_START + 1,
	MSG_ID_CLOSE = MSG_ID_START + 2,
	
	function msgRefreshEditor = 
	(
		
	),
	
	function msgCloseEditor = 
	(
		
	)

)

global mmdlmsg = MmdlWinMsg()

function saveToFile = 
(
	filename = getSaveFileName \ 
		caption:"Mmdl file export" \
		filename:(getDir #maxroot) \ 
		types:"mmdl (*.mmdl)|*.mmdl"

	print filename
)

rollout MmdlMainPanel "mmdl main panel" width:162 height:512
(
	label lbl3 "main panel" pos:[15,17] width:133 height:17


	button btn4 "浮动" pos:[13,47] width:53 height:36


	button btn11 "停靠" pos:[82,47] width:51 height:37


	button btn9 "保存" pos:[15,96] width:118 height:41


	edittext edt4 "" pos:[25,235] width:0 height:0


	label lbl7 "编辑器路径：" pos:[19,162] width:115 height:18


	edittext editorPathBox "" pos:[13,184] width:108 height:20


	button btn19 "..." pos:[126,183] width:20 height:21

	button btn20 "启动" pos:[83,211] width:61 height:25

	button btnSendMsg "Editor" pos:[17,260] width:63 height:53
	
	on MmdlMainPanel open do
	(
		g_mmdlConfig.readConfig()
	
		if(g_mmdlConfig.mEditorFullname.count > 0) then
			editorPathBox.text = g_mmdlConfig.mEditorFullname
	)
	on MmdlMainPanel okToClose do
	(
		print "mmdlMainPanel oktoclose"		
		
		g_mmdlMainFloater = undefined
	)
	on btn4 pressed do
	(
		cui.FloatDialogBar g_mmdlMainFloater
	)
	on btn11 pressed do
	(
		cui.DockDialogBar g_mmdlMainFloater cui_dock_left
	)
	on btn9 pressed do
	(
		saveToFile()
	)
	on btn19 pressed do -- set editor dir
	(
		startup_dir = getDir #maxroot
		editorPath = getSavePath caption:"请指定editor路径:" initialDir:startup_dir
		if(editorPath != undefined) then
		(
			exefilename = editorPath + "\\" + g_editorExeName;
			res = doesFileExist exefilename
			
			if(res != false) then
			(
				editorPathBox.text = exefilename
				
				g_mmdlConfig.mEditorFullname = exefilename
				g_mmdlConfig.saveConfig()
			)
			else
			(
				messagebox "editor 路径错误"			
			)
									
		)
	)
	on btn20 pressed do -- launch editor
	(
		if(g_mmdlConfig.mEditorFullname.count > 0 and
			(doesFileExist g_mmdlConfig.mEditorFullname) == true ) then
		(
			ShellLaunch g_mmdlConfig.mEditorFullname ""			
		)
	)
	
	on btnSendMsg pressed do
	(
-- 		if(ensureEditor() == true) then
-- 			sendEditorMessage mmdlmsg.MSG_ID_REFRESH 0
-- 		else
-- 			messagebox "editor 还没有启动"
		
		MmdlBridge testAdd 1 2
	)
)

function sendEditorMessage msg1 msg2 =
(
	msg = MmdlWinMsg()
	if(g_editorHWND != undefined) then
	(
		Windows.sendMessage g_editorHWND MSG_ID_MMDL msg1 msg2
	)
)

function getEditorHwnd editorname = 
(
	windows_hwnds = windows.getChildrenHWND 0
	for i = 1 to windows_hwnds.count do
	(
		hwnd = windows_hwnds[i]
		if(hwnd[5] == editorname) then
		(
			return hwnd[1]
		)
	)
	return undefined
)

function ensureEditor = 
(
	if(g_editorHWND == undefined) then
	(
		g_editorHWND = getEditorHwnd "mini editor"		
		if(g_editorHWND != undefined) then
		(
			return true			
		)
		else
		(
			--messagebox "editor 还没有启动"			
			return false
		)
	)	
	else
		return true
)

function openMainPanel = 
(
	if g_mmdlMainFloater == undefined do
	(
		g_mmdlConfig = MmdlConfig()
		
		g_mmdlMainFloater = newRolloutFloater "mmdl panel" 162 560		
			
		if g_mmdlMainFloater != undefined do 
			print "mmdlMainPanel created"
			
		addRollout MmdlMainPanel g_mmdlMainFloater		
			
		cui.RegisterDialogBar g_mmdlMainFloater		
		--cui.DockDialogBar g_mmdlMainFloater cui_dock_left 	--初始化时向窗体右边停靠
	)	
)

macroscript macroOpenMmdlFloater
buttontext:"打开主面板"
category:"MmdlExporter"
tooltip:"MmdlMainPanel"
(
	openMainPanel()
)

function setupMmdlMenu = 
(	
	local main_menu = menuMan.getMainMenuBar()
	
	g_mmdlMenu = menuMan.findMenu g_mmdlMenuName
	
	if g_mmdlMenu == undefined do	
		g_mmdlMenu = menuMan.createMenu g_mmdlMenuName
	
	local item_num = main_menu.numItems()
	
	
	for i = 1 to item_num do
	(
		local cur_item = main_menu.getItem i
		local item_title = cur_item.getTitle()
		
		print item_title
		
		if item_title == g_mmdlMenuName do
		(
			menuMan.updateMenuBar()	
			return false
		)					
	)

	local open_panel_item = menuMan.createActionItem "macroOpenMmdlFloater" "MmdlExporter"  
	
	g_mmdlMenu.addItem open_panel_item -1
	
	local mmdl_item = menuMan.createSubMenuItem "" g_mmdlMenu
	
	local item_pos = main_menu.numItems() - 1
	
	main_menu.addItem mmdl_item item_pos
				
	menuMan.updateMenuBar()	

	return true
)

if setupMmdlMenu() then 
	print "set mmdl menu ok "
else
	print "set mmdl menu failed"

 