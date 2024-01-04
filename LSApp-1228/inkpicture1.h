#pragma once

// ������������� Microsoft Visual C++ ������ IDispatch ��װ����

// ע��:  ��Ҫ�޸Ĵ��ļ������ݡ�  ���������
//  Microsoft Visual C++ �������ɣ������޸Ľ������ǡ�

/////////////////////////////////////////////////////////////////////////////
// CInkpicture1 ��װ����

class CInkpicture1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CInkpicture1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x4A1E553, 0xFE36, 0x4FDE, { 0x86, 0x5E, 0x34, 0x41, 0x94, 0xE6, 0x94, 0x24 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// ����
public:
enum
{
    ISC_FirstElement = 0,
    ISC_AllElements = -1
}ItemSelectionConstants;
enum
{
    IBBM_Default = 0,
    IBBM_NoCurveFit = 1,
    IBBM_CurveFit = 2,
    IBBM_PointsOnly = 3,
    IBBM_Union = 4
}InkBoundingBoxMode;
enum
{
    IEF_CopyFromOriginal = 0,
    IEF_RemoveFromOriginal = 1,
    IEF_Default = 1
}InkExtractFlags;
enum
{
    IPF_InkSerializedFormat = 0,
    IPF_Base64InkSerializedFormat = 1,
    IPF_GIF = 2,
    IPF_Base64GIF = 3
}InkPersistenceFormat;
enum
{
    IPCM_Default = 0,
    IPCM_MaximumCompression = 1,
    IPCM_NoCompression = 2
}InkPersistenceCompressionMode;
enum
{
    IPT_Ball = 0,
    IPT_Rectangle = 1
}InkPenTip;
enum
{
    IRO_Black = 1,
    IRO_NotMergePen = 2,
    IRO_MaskNotPen = 3,
    IRO_NotCopyPen = 4,
    IRO_MaskPenNot = 5,
    IRO_Not = 6,
    IRO_XOrPen = 7,
    IRO_NotMaskPen = 8,
    IRO_MaskPen = 9,
    IRO_NotXOrPen = 10,
    IRO_NoOperation = 11,
    IRO_MergeNotPen = 12,
    IRO_CopyPen = 13,
    IRO_MergePenNot = 14,
    IRO_MergePen = 15,
    IRO_White = 16
}InkRasterOperation;
enum
{
    IMP_Default = 0,
    IMP_Arrow = 1,
    IMP_Crosshair = 2,
    IMP_Ibeam = 3,
    IMP_SizeNESW = 4,
    IMP_SizeNS = 5,
    IMP_SizeNWSE = 6,
    IMP_SizeWE = 7,
    IMP_UpArrow = 8,
    IMP_Hourglass = 9,
    IMP_NoDrop = 10,
    IMP_ArrowHourglass = 11,
    IMP_ArrowQuestion = 12,
    IMP_SizeAll = 13,
    IMP_Hand = 14,
    IMP_Custom = 99
}InkMousePointer;
enum
{
    ICB_Copy = 0,
    ICB_Cut = 1,
    ICB_ExtractOnly = 48,
    ICB_DelayedCopy = 32,
    ICB_Default = 0
}InkClipboardModes;
enum
{
    ICF_None = 0,
    ICF_InkSerializedFormat = 1,
    ICF_SketchInk = 2,
    ICF_TextInk = 6,
    ICF_EnhancedMetafile = 8,
    ICF_Metafile = 32,
    ICF_Bitmap = 64,
    ICF_PasteMask = 7,
    ICF_CopyMask = 127,
    ICF_Default = 127
}InkClipboardFormats;
enum
{
    SHR_None = 0,
    SHR_NW = 1,
    SHR_SE = 2,
    SHR_NE = 3,
    SHR_SW = 4,
    SHR_E = 5,
    SHR_W = 6,
    SHR_N = 7,
    SHR_S = 8,
    SHR_Selection = 9
}SelectionHitResult;
enum
{
    IRS_NoError = 0,
    IRS_Interrupted = 1,
    IRS_ProcessFailed = 2,
    IRS_InkAddedFailed = 4,
    IRS_SetAutoCompletionModeFailed = 8,
    IRS_SetStrokesFailed = 16,
    IRS_SetGuideFailed = 32,
    IRS_SetFlagsFailed = 64,
    IRS_SetFactoidFailed = 128,
    IRS_SetPrefixSuffixFailed = 256,
    IRS_SetWordListFailed = 512
}InkRecognitionStatus;
enum
{
    DISPID_IRTop = 1,
    DISPID_IRLeft = 2,
    DISPID_IRBottom = 3,
    DISPID_IRRight = 4,
    DISPID_IRGetRectangle = 5,
    DISPID_IRSetRectangle = 6,
    DISPID_IRData = 7
}DISPID_InkRectangle;
enum
{
    DISPID_IEPGuid = 1,
    DISPID_IEPData = 2
}DISPID_InkExtendedProperty;
enum
{
    DISPID_IEPs_NewEnum = -4,
    DISPID_IEPsItem = 0,
    DISPID_IEPsCount = 1,
    DISPID_IEPsAdd = 2,
    DISPID_IEPsRemove = 3,
    DISPID_IEPsClear = 4,
    DISPID_IEPsDoesPropertyExist = 5
}DISPID_InkExtendedProperties;
enum
{
    DISPID_DAHeight = 1,
    DISPID_DAColor = 2,
    DISPID_DAWidth = 3,
    DISPID_DAFitToCurve = 4,
    DISPID_DAIgnorePressure = 5,
    DISPID_DAAntiAliased = 6,
    DISPID_DATransparency = 7,
    DISPID_DARasterOperation = 8,
    DISPID_DAPenTip = 9,
    DISPID_DAClone = 10,
    DISPID_DAExtendedProperties = 11
}DISPID_InkDrawingAttributes;
enum
{
    DISPID_ITReset = 1,
    DISPID_ITTranslate = 2,
    DISPID_ITRotate = 3,
    DISPID_ITReflect = 4,
    DISPID_ITShear = 5,
    DISPID_ITScale = 6,
    DISPID_ITeM11 = 7,
    DISPID_ITeM12 = 8,
    DISPID_ITeM21 = 9,
    DISPID_ITeM22 = 10,
    DISPID_ITeDx = 11,
    DISPID_ITeDy = 12,
    DISPID_ITGetTransform = 13,
    DISPID_ITSetTransform = 14,
    DISPID_ITData = 15
}DISPID_InkTransform;
enum
{
    IAG_AllGestures = 0,
    IAG_NoGesture = 61440,
    IAG_Scratchout = 61441,
    IAG_Triangle = 61442,
    IAG_Square = 61443,
    IAG_Star = 61444,
    IAG_Check = 61445,
    IAG_Curlicue = 61456,
    IAG_DoubleCurlicue = 61457,
    IAG_Circle = 61472,
    IAG_DoubleCircle = 61473,
    IAG_SemiCircleLeft = 61480,
    IAG_SemiCircleRight = 61481,
    IAG_ChevronUp = 61488,
    IAG_ChevronDown = 61489,
    IAG_ChevronLeft = 61490,
    IAG_ChevronRight = 61491,
    IAG_ArrowUp = 61496,
    IAG_ArrowDown = 61497,
    IAG_ArrowLeft = 61498,
    IAG_ArrowRight = 61499,
    IAG_Up = 61528,
    IAG_Down = 61529,
    IAG_Left = 61530,
    IAG_Right = 61531,
    IAG_UpDown = 61536,
    IAG_DownUp = 61537,
    IAG_LeftRight = 61538,
    IAG_RightLeft = 61539,
    IAG_UpLeftLong = 61540,
    IAG_UpRightLong = 61541,
    IAG_DownLeftLong = 61542,
    IAG_DownRightLong = 61543,
    IAG_UpLeft = 61544,
    IAG_UpRight = 61545,
    IAG_DownLeft = 61546,
    IAG_DownRight = 61547,
    IAG_LeftUp = 61548,
    IAG_LeftDown = 61549,
    IAG_RightUp = 61550,
    IAG_RightDown = 61551,
    IAG_Exclamation = 61604,
    IAG_Tap = 61680,
    IAG_DoubleTap = 61681
}InkApplicationGesture;
enum
{
    ISG_Tap = 16,
    ISG_DoubleTap = 17,
    ISG_RightTap = 18,
    ISG_Drag = 19,
    ISG_RightDrag = 20,
    ISG_HoldEnter = 21,
    ISG_HoldLeave = 22,
    ISG_HoverEnter = 23,
    ISG_HoverLeave = 24,
    ISG_Flick = 31
}InkSystemGesture;
enum
{
    IRC_Strong = 0,
    IRC_Intermediate = 1,
    IRC_Poor = 2
}InkRecognitionConfidence;
enum
{
    DISPID_IGId = 0,
    DISPID_IGGetHotPoint = 1,
    DISPID_IGConfidence = 2
}DISPID_InkGesture;
enum
{
    DISPID_ICsrName = 0,
    DISPID_ICsrId = 1,
    DISPID_ICsrDrawingAttributes = 2,
    DISPID_ICsrButtons = 3,
    DISPID_ICsrInverted = 4,
    DISPID_ICsrTablet = 5
}DISPID_InkCursor;
enum
{
    THWC_Integrated = 1,
    THWC_CursorMustTouch = 2,
    THWC_HardProximity = 4,
    THWC_CursorsHavePhysicalIds = 8
}TabletHardwareCapabilities;
enum
{
    TPMU_Default = 0,
    TPMU_Inches = 1,
    TPMU_Centimeters = 2,
    TPMU_Degrees = 3,
    TPMU_Radians = 4,
    TPMU_Seconds = 5,
    TPMU_Pounds = 6,
    TPMU_Grams = 7
}TabletPropertyMetricUnit;
enum
{
    ICBS_Unavailable = 0,
    ICBS_Up = 1,
    ICBS_Down = 2
}InkCursorButtonState;
enum
{
    DISPID_ICs_NewEnum = -4,
    DISPID_ICsItem = 0,
    DISPID_ICsCount = 1
}DISPID_InkCursors;
enum
{
    DISPID_ICBName = 0,
    DISPID_ICBId = 1,
    DISPID_ICBState = 2
}DISPID_InkCursorButton;
enum
{
    DISPID_ICBs_NewEnum = -4,
    DISPID_ICBsItem = 0,
    DISPID_ICBsCount = 1
}DISPID_InkCursorButtons;
enum
{
    DISPID_ITName = 0,
    DISPID_ITPlugAndPlayId = 1,
    DISPID_ITPropertyMetrics = 2,
    DISPID_ITIsPacketPropertySupported = 3,
    DISPID_ITMaximumInputRectangle = 4,
    DISPID_ITHardwareCapabilities = 5
}DISPID_InkTablet;
enum
{
    TDK_Mouse = 0,
    TDK_Pen = 1,
    TDK_Touch = 2
}TabletDeviceKind;
enum
{
    DISPID_IT2DeviceKind = 0
}DISPID_InkTablet2;
enum
{
    DISPID_IT3IsMultiTouch = 0,
    DISPID_IT3MaximumCursors = 1
}DISPID_InkTablet3;
enum
{
    DISPID_ITs_NewEnum = -4,
    DISPID_ITsItem = 0,
    DISPID_ITsDefaultTablet = 1,
    DISPID_ITsCount = 2,
    DISPID_ITsIsPacketPropertySupported = 3
}DISPID_InkTablets;
enum
{
    DISPID_ISDInkIndex = 1,
    DISPID_ISDID = 2,
    DISPID_ISDGetBoundingBox = 3,
    DISPID_ISDDrawingAttributes = 4,
    DISPID_ISDFindIntersections = 5,
    DISPID_ISDGetRectangleIntersections = 6,
    DISPID_ISDClip = 7,
    DISPID_ISDHitTestCircle = 8,
    DISPID_ISDNearestPoint = 9,
    DISPID_ISDSplit = 10,
    DISPID_ISDExtendedProperties = 11,
    DISPID_ISDInk = 12,
    DISPID_ISDBezierPoints = 13,
    DISPID_ISDPolylineCusps = 14,
    DISPID_ISDBezierCusps = 15,
    DISPID_ISDSelfIntersections = 16,
    DISPID_ISDPacketCount = 17,
    DISPID_ISDPacketSize = 18,
    DISPID_ISDPacketDescription = 19,
    DISPID_ISDDeleted = 20,
    DISPID_ISDGetPacketDescriptionPropertyMetrics = 21,
    DISPID_ISDGetPoints = 22,
    DISPID_ISDSetPoints = 23,
    DISPID_ISDGetPacketData = 24,
    DISPID_ISDGetPacketValuesByProperty = 25,
    DISPID_ISDSetPacketValuesByProperty = 26,
    DISPID_ISDGetFlattenedBezierPoints = 27,
    DISPID_ISDScaleToRectangle = 28,
    DISPID_ISDTransform = 29,
    DISPID_ISDMove = 30,
    DISPID_ISDRotate = 31,
    DISPID_ISDShear = 32,
    DISPID_ISDScale = 33
}DISPID_InkStrokeDisp;
enum
{
    DISPID_ISs_NewEnum = -4,
    DISPID_ISsItem = 0,
    DISPID_ISsCount = 1,
    DISPID_ISsValid = 2,
    DISPID_ISsInk = 3,
    DISPID_ISsAdd = 4,
    DISPID_ISsAddStrokes = 5,
    DISPID_ISsRemove = 6,
    DISPID_ISsRemoveStrokes = 7,
    DISPID_ISsToString = 8,
    DISPID_ISsModifyDrawingAttributes = 9,
    DISPID_ISsGetBoundingBox = 10,
    DISPID_ISsScaleToRectangle = 11,
    DISPID_ISsTransform = 12,
    DISPID_ISsMove = 13,
    DISPID_ISsRotate = 14,
    DISPID_ISsShear = 15,
    DISPID_ISsScale = 16,
    DISPID_ISsClip = 17,
    DISPID_ISsRecognitionResult = 18,
    DISPID_ISsRemoveRecognitionResult = 19
}DISPID_InkStrokes;
enum
{
    DISPID_ICSs_NewEnum = -4,
    DISPID_ICSsItem = 0,
    DISPID_ICSsCount = 1,
    DISPID_ICSsAdd = 2,
    DISPID_ICSsRemove = 3,
    DISPID_ICSsClear = 4
}DISPID_InkCustomStrokes;
enum
{
    DISPID_SEStrokesAdded = 1,
    DISPID_SEStrokesRemoved = 2
}DISPID_StrokeEvent;
enum
{
    DISPID_IStrokes = 1,
    DISPID_IExtendedProperties = 2,
    DISPID_IGetBoundingBox = 3,
    DISPID_IDeleteStrokes = 4,
    DISPID_IDeleteStroke = 5,
    DISPID_IExtractStrokes = 6,
    DISPID_IExtractWithRectangle = 7,
    DISPID_IDirty = 8,
    DISPID_ICustomStrokes = 9,
    DISPID_IClone = 10,
    DISPID_IHitTestCircle = 11,
    DISPID_IHitTestWithRectangle = 12,
    DISPID_IHitTestWithLasso = 13,
    DISPID_INearestPoint = 14,
    DISPID_ICreateStrokes = 15,
    DISPID_ICreateStroke = 16,
    DISPID_IAddStrokesAtRectangle = 17,
    DISPID_IClip = 18,
    DISPID_ISave = 19,
    DISPID_ILoad = 20,
    DISPID_ICreateStrokeFromPoints = 21,
    DISPID_IClipboardCopyWithRectangle = 22,
    DISPID_IClipboardCopy = 23,
    DISPID_ICanPaste = 24,
    DISPID_IClipboardPaste = 25
}DISPID_Ink;
enum
{
    DISPID_IEInkAdded = 1,
    DISPID_IEInkDeleted = 2
}DISPID_InkEvent;
enum
{
    DISPID_IRGetViewTransform = 1,
    DISPID_IRSetViewTransform = 2,
    DISPID_IRGetObjectTransform = 3,
    DISPID_IRSetObjectTransform = 4,
    DISPID_IRDraw = 5,
    DISPID_IRDrawStroke = 6,
    DISPID_IRPixelToInkSpace = 7,
    DISPID_IRInkSpaceToPixel = 8,
    DISPID_IRPixelToInkSpaceFromPoints = 9,
    DISPID_IRInkSpaceToPixelFromPoints = 10,
    DISPID_IRMeasure = 11,
    DISPID_IRMeasureStroke = 12,
    DISPID_IRMove = 13,
    DISPID_IRRotate = 14,
    DISPID_IRScale = 15
}DISPID_InkRenderer;
enum
{
    ICEI_DefaultEvents = -1,
    ICEI_CursorDown = 0,
    ICEI_Stroke = 1,
    ICEI_NewPackets = 2,
    ICEI_NewInAirPackets = 3,
    ICEI_CursorButtonDown = 4,
    ICEI_CursorButtonUp = 5,
    ICEI_CursorInRange = 6,
    ICEI_CursorOutOfRange = 7,
    ICEI_SystemGesture = 8,
    ICEI_TabletAdded = 9,
    ICEI_TabletRemoved = 10,
    ICEI_MouseDown = 11,
    ICEI_MouseMove = 12,
    ICEI_MouseUp = 13,
    ICEI_MouseWheel = 14,
    ICEI_DblClick = 15,
    ICEI_AllEvents = 16
}InkCollectorEventInterest;
enum
{
    IMF_Left = 1,
    IMF_Right = 2,
    IMF_Middle = 4
}InkMouseButton;
enum
{
    IKM_Shift = 1,
    IKM_Control = 2,
    IKM_Alt = 4
}InkShiftKeyModifierFlags;
enum
{
    DISPID_ICEStroke = 1,
    DISPID_ICECursorDown = 2,
    DISPID_ICENewPackets = 3,
    DISPID_ICENewInAirPackets = 4,
    DISPID_ICECursorButtonDown = 5,
    DISPID_ICECursorButtonUp = 6,
    DISPID_ICECursorInRange = 7,
    DISPID_ICECursorOutOfRange = 8,
    DISPID_ICESystemGesture = 9,
    DISPID_ICEGesture = 10,
    DISPID_ICETabletAdded = 11,
    DISPID_ICETabletRemoved = 12,
    DISPID_IOEPainting = 13,
    DISPID_IOEPainted = 14,
    DISPID_IOESelectionChanging = 15,
    DISPID_IOESelectionChanged = 16,
    DISPID_IOESelectionMoving = 17,
    DISPID_IOESelectionMoved = 18,
    DISPID_IOESelectionResizing = 19,
    DISPID_IOESelectionResized = 20,
    DISPID_IOEStrokesDeleting = 21,
    DISPID_IOEStrokesDeleted = 22,
    DISPID_IPEChangeUICues = 23,
    DISPID_IPEClick = 24,
    DISPID_IPEDblClick = 25,
    DISPID_IPEInvalidated = 26,
    DISPID_IPEMouseDown = 27,
    DISPID_IPEMouseEnter = 28,
    DISPID_IPEMouseHover = 29,
    DISPID_IPEMouseLeave = 30,
    DISPID_IPEMouseMove = 31,
    DISPID_IPEMouseUp = 32,
    DISPID_IPEMouseWheel = 33,
    DISPID_IPESizeModeChanged = 34,
    DISPID_IPEStyleChanged = 35,
    DISPID_IPESystemColorsChanged = 36,
    DISPID_IPEKeyDown = 37,
    DISPID_IPEKeyPress = 38,
    DISPID_IPEKeyUp = 39,
    DISPID_IPEResize = 40,
    DISPID_IPESizeChanged = 41
}DISPID_InkCollectorEvent;
enum
{
    IOEM_Ink = 0,
    IOEM_Delete = 1,
    IOEM_Select = 2
}InkOverlayEditingMode;
enum
{
    IOAM_Behind = 0,
    IOAM_InFront = 1
}InkOverlayAttachMode;
enum
{
    IPSM_AutoSize = 0,
    IPSM_CenterImage = 1,
    IPSM_Normal = 2,
    IPSM_StretchImage = 3
}InkPictureSizeMode;
enum
{
    IOERM_StrokeErase = 0,
    IOERM_PointErase = 1
}InkOverlayEraserMode;
enum
{
    ICM_InkOnly = 0,
    ICM_GestureOnly = 1,
    ICM_InkAndGesture = 2
}InkCollectionMode;
enum
{
    DISPID_ICEnabled = 1,
    DISPID_ICHwnd = 2,
    DISPID_ICPaint = 3,
    DISPID_ICText = 4,
    DISPID_ICDefaultDrawingAttributes = 5,
    DISPID_ICRenderer = 6,
    DISPID_ICInk = 7,
    DISPID_ICAutoRedraw = 8,
    DISPID_ICCollectingInk = 9,
    DISPID_ICSetEventInterest = 10,
    DISPID_ICGetEventInterest = 11,
    DISPID_IOEditingMode = 12,
    DISPID_IOSelection = 13,
    DISPID_IOAttachMode = 14,
    DISPID_IOHitTestSelection = 15,
    DISPID_IODraw = 16,
    DISPID_IPPicture = 17,
    DISPID_IPSizeMode = 18,
    DISPID_IPBackColor = 19,
    DISPID_ICCursors = 20,
    DISPID_ICMarginX = 21,
    DISPID_ICMarginY = 22,
    DISPID_ICSetWindowInputRectangle = 23,
    DISPID_ICGetWindowInputRectangle = 24,
    DISPID_ICTablet = 25,
    DISPID_ICSetAllTabletsMode = 26,
    DISPID_ICSetSingleTabletIntegratedMode = 27,
    DISPID_ICCollectionMode = 28,
    DISPID_ICSetGestureStatus = 29,
    DISPID_ICGetGestureStatus = 30,
    DISPID_ICDynamicRendering = 31,
    DISPID_ICDesiredPacketDescription = 32,
    DISPID_IOEraserMode = 33,
    DISPID_IOEraserWidth = 34,
    DISPID_ICMouseIcon = 35,
    DISPID_ICMousePointer = 36,
    DISPID_IPInkEnabled = 37,
    DISPID_ICSupportHighContrastInk = 38,
    DISPID_IOSupportHighContrastSelectionUI = 39
}DISPID_InkCollector;
enum
{
    DISPID_RecoClsid = 1,
    DISPID_RecoName = 2,
    DISPID_RecoVendor = 3,
    DISPID_RecoCapabilities = 4,
    DISPID_RecoLanguageID = 5,
    DISPID_RecoPreferredPacketDescription = 6,
    DISPID_RecoCreateRecognizerContext = 7,
    DISPID_RecoSupportedProperties = 8
}DISPID_InkRecognizer;
enum
{
    IRC_DontCare = 1,
    IRC_Object = 2,
    IRC_FreeInput = 4,
    IRC_LinedInput = 8,
    IRC_BoxedInput = 16,
    IRC_CharacterAutoCompletionInput = 32,
    IRC_RightAndDown = 64,
    IRC_LeftAndDown = 128,
    IRC_DownAndLeft = 256,
    IRC_DownAndRight = 512,
    IRC_ArbitraryAngle = 1024,
    IRC_Lattice = 2048,
    IRC_AdviseInkChange = 4096,
    IRC_StrokeReorder = 8192,
    IRC_Personalizable = 16384,
    IRC_PrefersArbitraryAngle = 32768,
    IRC_PrefersParagraphBreaking = 65536,
    IRC_PrefersSegmentation = 131072,
    IRC_Cursive = 262144,
    IRC_TextPrediction = 524288,
    IRC_Alpha = 1048576,
    IRC_Beta = 2097152
}InkRecognizerCapabilities;
enum
{
    IRCACM_Full = 0,
    IRCACM_Prefix = 1,
    IRCACM_Random = 2
}InkRecognizerCharacterAutoCompletionMode;
enum
{
    IRM_None = 0,
    IRM_WordModeOnly = 1,
    IRM_Coerce = 2,
    IRM_TopInkBreaksOnly = 4,
    IRM_PrefixOk = 8,
    IRM_LineMode = 16,
    IRM_DisablePersonalization = 32,
    IRM_AutoSpace = 64,
    IRM_Max = 128
}InkRecognitionModes;
enum
{
    DISPID_RecoId = 0,
    DISPID_RecoUnicodeRanges = 1
}DISPID_InkRecognizer2;
enum
{
    DISPID_IRecos_NewEnum = -4,
    DISPID_IRecosItem = 0,
    DISPID_IRecosCount = 1,
    DISPID_IRecosGetDefaultRecognizer = 2
}DISPID_InkRecognizers;
enum
{
    DISPID_IRERecognitionWithAlternates = 1,
    DISPID_IRERecognition = 2
}DISPID_InkRecognitionEvent;
enum
{
    DISPID_IRecoCtx_Strokes = 1,
    DISPID_IRecoCtx_CharacterAutoCompletionMode = 2,
    DISPID_IRecoCtx_Factoid = 3,
    DISPID_IRecoCtx_WordList = 4,
    DISPID_IRecoCtx_Recognizer = 5,
    DISPID_IRecoCtx_Guide = 6,
    DISPID_IRecoCtx_Flags = 7,
    DISPID_IRecoCtx_PrefixText = 8,
    DISPID_IRecoCtx_SuffixText = 9,
    DISPID_IRecoCtx_StopRecognition = 10,
    DISPID_IRecoCtx_Clone = 11,
    DISPID_IRecoCtx_Recognize = 12,
    DISPID_IRecoCtx_StopBackgroundRecognition = 13,
    DISPID_IRecoCtx_EndInkInput = 14,
    DISPID_IRecoCtx_BackgroundRecognize = 15,
    DISPID_IRecoCtx_BackgroundRecognizeWithAlternates = 16,
    DISPID_IRecoCtx_IsStringSupported = 17
}DISPID_InkRecoContext;
enum
{
    DISPID_IRecoCtx2_EnabledUnicodeRanges = 0
}DISPID_InkRecoContext2;
enum
{
    IRAS_Start = 0,
    IRAS_DefaultCount = 10,
    IRAS_All = -1
}InkRecognitionAlternatesSelection;
enum
{
    DISPID_InkRecognitionResult_TopString = 1,
    DISPID_InkRecognitionResult_TopAlternate = 2,
    DISPID_InkRecognitionResult_Strokes = 3,
    DISPID_InkRecognitionResult_TopConfidence = 4,
    DISPID_InkRecognitionResult_AlternatesFromSelection = 5,
    DISPID_InkRecognitionResult_ModifyTopAlternate = 6,
    DISPID_InkRecognitionResult_SetResultOnStrokes = 7
}DISPID_InkRecognitionResult;
enum
{
    DISPID_InkRecoAlternate_String = 1,
    DISPID_InkRecoAlternate_LineNumber = 2,
    DISPID_InkRecoAlternate_Baseline = 3,
    DISPID_InkRecoAlternate_Midline = 4,
    DISPID_InkRecoAlternate_Ascender = 5,
    DISPID_InkRecoAlternate_Descender = 6,
    DISPID_InkRecoAlternate_Confidence = 7,
    DISPID_InkRecoAlternate_Strokes = 8,
    DISPID_InkRecoAlternate_GetStrokesFromStrokeRanges = 9,
    DISPID_InkRecoAlternate_GetStrokesFromTextRange = 10,
    DISPID_InkRecoAlternate_GetTextRangeFromStrokes = 11,
    DISPID_InkRecoAlternate_GetPropertyValue = 12,
    DISPID_InkRecoAlternate_LineAlternates = 13,
    DISPID_InkRecoAlternate_ConfidenceAlternates = 14,
    DISPID_InkRecoAlternate_AlternatesWithConstantPropertyValues = 15
}DISPID_InkRecoAlternate;
enum
{
    DISPID_InkRecognitionAlternates_NewEnum = -4,
    DISPID_InkRecognitionAlternates_Item = 0,
    DISPID_InkRecognitionAlternates_Count = 1,
    DISPID_InkRecognitionAlternates_Strokes = 2
}DISPID_InkRecognitionAlternates;
enum
{
    DISPID_IRGWritingBox = 1,
    DISPID_IRGDrawnBox = 2,
    DISPID_IRGRows = 3,
    DISPID_IRGColumns = 4,
    DISPID_IRGMidline = 5,
    DISPID_IRGGuideData = 6
}DISPID_InkRecognizerGuide;
enum
{
    DISPID_InkWordList_AddWord = 0,
    DISPID_InkWordList_RemoveWord = 1,
    DISPID_InkWordList_Merge = 2
}DISPID_InkWordList;
enum
{
    DISPID_InkWordList2_AddWords = 3
}DISPID_InkWordList2;


// ����
public:

// IInkPicture

// Functions
//

	long get_hWnd()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DefaultDrawingAttributes()
	{
		LPDISPATCH result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_DefaultDrawingAttributes(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Renderer()
	{
		LPDISPATCH result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_Renderer(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Ink()
	{
		LPDISPATCH result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_Ink(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x7, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AutoRedraw()
	{
		BOOL result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AutoRedraw(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_CollectingInk()
	{
		BOOL result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_CollectionMode()
	{
		long result;
		InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CollectionMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DynamicRendering()
	{
		BOOL result;
		InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DynamicRendering(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_DesiredPacketDescription()
	{
		VARIANT result;
		InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_DesiredPacketDescription(VARIANT newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH get_MouseIcon()
	{
		LPDISPATCH result;
		InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_MouseIcon(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void putref_MouseIcon(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x23, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MousePointer()
	{
		long result;
		InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MousePointer(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EditingMode()
	{
		long result;
		InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EditingMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Selection()
	{
		LPDISPATCH result;
		InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_Selection(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EraserMode()
	{
		long result;
		InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EraserMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EraserWidth()
	{
		long result;
		InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EraserWidth(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void putref_Picture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x11, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	void put_Picture(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Picture()
	{
		LPDISPATCH result;
		InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_SizeMode(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_SizeMode()
	{
		long result;
		InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BackColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackColor()
	{
		unsigned long result;
		InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Cursors()
	{
		LPDISPATCH result;
		InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_MarginX()
	{
		long result;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MarginX(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_MarginY()
	{
		long result;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_MarginY(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Tablet()
	{
		LPDISPATCH result;
		InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_SupportHighContrastInk()
	{
		BOOL result;
		InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_SupportHighContrastInk(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_SupportHighContrastSelectionUI()
	{
		BOOL result;
		InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_SupportHighContrastSelectionUI(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long HitTestSelection(long x, long y)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, x, y);
		return result;
	}
	void SetGestureStatus(long Gesture, BOOL Listen)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Gesture, Listen);
	}
	BOOL GetGestureStatus(long Gesture)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Gesture);
		return result;
	}
	void GetWindowInputRectangle(LPDISPATCH * WindowInputRectangle)
	{
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms, WindowInputRectangle);
	}
	void SetWindowInputRectangle(LPDISPATCH WindowInputRectangle)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms, WindowInputRectangle);
	}
	void SetAllTabletsMode(BOOL UseMouseForInput)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, UseMouseForInput);
	}
	void SetSingleTabletIntegratedMode(LPDISPATCH Tablet)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Tablet);
	}
	BOOL GetEventInterest(long EventId)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, EventId);
		return result;
	}
	void SetEventInterest(long EventId, BOOL Listen)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EventId, Listen);
	}
	BOOL get_InkEnabled()
	{
		BOOL result;
		InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_InkEnabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Enabled()
	{
		BOOL result;
		InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Enabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

// Properties
//



};
