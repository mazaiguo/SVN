//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "BlkJig.h"

//-----------------------------------------------------------------------------
CBlkJig::CBlkJig () : AcEdJig (),
	mCurrentInputLevel(0), mpEntity(NULL)
{
	m_bRotate = false;
	m_bScale = false;
	m_scale= 1.0;;
	m_dRotate = 0;
}

CBlkJig::~CBlkJig () {
}

//-----------------------------------------------------------------------------
AcEdJig::DragStatus CBlkJig::startJig () 
{
	//- Store the new entity pointer
	mpEntity = new AcDbBlockReference;
	mpEntity->setDatabaseDefaults();
	mpEntity->setBlockTableRecord(m_blockId);
	mpEntity->setScaleFactors(AcGeScale3d(m_scale, m_scale, m_scale));
	mpEntity->setRotation(m_dRotate);

	int nCount = 0;
	if (m_bRotate && m_bScale)
	{
		nCount = 3;
	}
	else if (m_bRotate && (!m_bScale))
	{
		nCount = 2;
	}
	else if ((!m_bRotate) && m_bScale)
	{
		nCount = 2;
	}
	else 
	{
		nCount = 1;
	}
	//- Setup each input prompt
	AcString inputPrompts [3] ={
		"\n请指定插入点",
		"\n请输入旋转角度",
		"\n请输入比例"
	} ;
	//- Setup kwords for each input
// 	AcString kwords [3] ={
// 		"",
// 		"",
// 		""
// 	} ;

	bool appendOk =true ;
	AcEdJig::DragStatus status =AcEdJig::kNull ;
	//- Loop the number of inputs
	for ( mCurrentInputLevel =0 ; mCurrentInputLevel < nCount ; mCurrentInputLevel++ ) {
		//- Add a new input point to the list of input points
		mInputPoints.append (AcGePoint3d ()) ;
		//- Set the input prompt
		setDispPrompt (inputPrompts [mCurrentInputLevel]) ;
		//- Setup the keywords required
		//setKeywordList (kwords [mCurrentInputLevel]) ;

		bool quit =false ;
		//- Lets now do the input
		status =drag () ;
		if ( status != kNormal ) {
			//- If it's a keyword
			switch ( status ) {
				case kCancel: 
				case kNull:
					quit =true ;
					break ;

				case kKW1:
				case kKW2:
				case kKW3:
				case kKW4:
				case kKW5:
				case kKW6:
				case kKW7:
				case kKW8:
				case kKW9:
					//- Do something

					break ;
		  }
		} 
		else 
		{
			appendOk =true ;
		}

		//- If to finish
		if ( quit )
			break ;
	}

	//- If the input went well
	if ( appendOk )
		//- Append to the database
		append () ;
	else
		//- Clean up
		delete mpEntity  ;

	return (status) ;
}

//-----------------------------------------------------------------------------
//- Input sampler
AcEdJig::DragStatus CBlkJig::sampler () {
	//- Setup the user input controls for each input
	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
		| AcEdJig::kNoNegativeResponseAccepted
		| AcEdJig::kNullResponseAccepted ));

	AcGePoint3d rotatePt;	

	AcEdJig::DragStatus stat =AcEdJig::kCancel ;
	//- Check the current input number to see which input to do
	switch ( mCurrentInputLevel+1 ) {
		case 1:
			// TODO : get an input here
			//status =GetStartPoint () ;
			stat = acquirePoint(m_insertPt);
			if (rotatePt != m_insertPt)
			{
				rotatePt = m_insertPt;
			}
			else if (stat == AcEdJig::kNormal)
			{
				return AcEdJig::kNoChange;
			}
			break ;
		case 2:
			// TODO : get an input here
			//status =GetStartPoint () ;
			static double dRotate;
			stat = acquireAngle(m_dRotate, m_insertPt);		
			if (dRotate != m_dRotate)
			{
				dRotate = m_dRotate;
			}
			else if (stat == AcEdJig::kNormal)
			{
				return AcEdJig::kNoChange;
			}
			break ;
		case  3:
			static double dScale;
			stat = acquireDist(m_scale);
			if (dScale != m_scale)
			{
				dScale = m_scale;
			}
			else if (stat == AcEdJig::kNormal)
			{
				return AcEdJig::kNoChange;
			}
			break;
		default:
			break ;
	}
	return (stat) ;
}

//-----------------------------------------------------------------------------
//- Jigged entity update
Adesk::Boolean CBlkJig::update () {
	//- Check the current input number to see which update to do
	switch ( mCurrentInputLevel+1 ) {
		case 1:
			// TODO : update your entity for this input
			//mpEntity->setCenter (mInputPoints [mCurrentInputLevel]) ;
			mpEntity->setPosition(m_insertPt);
			break ;
		case 2:
			// TODO : update your entity for this input
			//mpEntity->setCenter (mInputPoints [mCurrentInputLevel]) ;
			mpEntity->setRotation(m_dRotate);
			break ;
		case 3:
			mpEntity->setScaleFactors(AcGeScale3d(m_scale, m_scale, m_scale));
			break;
		default:
			break ;
	}

	return (updateDimData ()) ;
}

//-----------------------------------------------------------------------------
//- Jigged entity pointer return
AcDbEntity *CBlkJig::entity () const {
	return ((AcDbEntity *)mpEntity) ;
}

//-----------------------------------------------------------------------------
//- Dynamic dimension data setup
AcDbDimDataPtrArray *CBlkJig::dimData (const double dimScale) {

	/* SAMPLE CODE:
	AcDbAlignedDimension *dim =new AcDbAlignedDimension () ;
	dim->setDatabaseDefaults () ;
	dim->setNormal (AcGeVector3d::kZAxis) ;
	dim->setElevation (0.0) ;
	dim->setHorizontalRotation (0.0) ;
	dim->setXLine1Point (m_originPoint) ;
	dim->setXLine2Point (m_lastPoint) ;
	//- Get the dimPoint, first the midpoint
	AcGePoint3d dimPoint =m_originPoint + ((m_lastPoint - m_originPoint) / 2.0) ;
	//- Then the offset
	dim->setDimLinePoint (dimPoint) ;
	dim->setDimtad (1) ;

	AcDbDimData *dimData = new AcDbDimData (dim) ;
	//AppData *appData =new AppData (1, dimScale) ;
	//dimData.setAppData (appData) ;
	dimData->setDimFocal (true) ;
	dimData->setDimHideIfValueIsZero (true) ;

	//- Check to see if it is required
	if ( getDynDimensionRequired (m_inputNumber) )
		dimData->setDimInvisible (false) ;
	else
		dimData->setDimInvisible (true) ;

	//- Make sure it is editable TODO: 
	dimData->setDimEditable (true) ;
	mDimData.append (dimData) ;

	return (&mDimData) ;
	*/
	return (NULL) ;
}

//-----------------------------------------------------------------------------
//- Dynamic dimension data update
Acad::ErrorStatus CBlkJig::setDimValue (const AcDbDimData *pDimData, const double dimValue) {
	Acad::ErrorStatus es =Acad::eOk ;

	/* SAMPLE CODE:
	//- Convert the const pointer to non const
	AcDbDimData *dimDataNC =const_cast<AcDbDimData *>(pDimData) ;
	int inputNumber =-1 ;
	//- Find the dim data being passed so we can determine the input number
	if ( mDimData.find (dimDataNC, inputNumber) ) {
		//- Now get the dimension
		AcDbDimension *pDim =(AcDbDimension *)dimDataNC->dimension () ;
		//- Check it's the type of dimension we want
		AcDbAlignedDimension *pAlnDim =AcDbAlignedDimension::cast (pDim) ;
		//- If ok
		if ( pAlnDim ) {
			//- Extract the dimensions as they are now
			AcGePoint3d dimStart =pAlnDim->xLine1Point () ;
			AcGePoint3d dimEnd =pAlnDim->xLine2Point () ;
			//- Lets get the new point entered by the user 
			AcGePoint3d dimEndNew =dimStart + (dimEnd - dimStart).normalize () * dimValue ;
			//- Finally set the end dim point
			pAlnDim->setXLine2Point (dimEndNew) ;
			//- Now update the jig data to reflect the dynamic dimension input
			mInputPoints [mCurrentInputLevel] =dimEndNew ;
		}
	}*/
	return (es) ;
}

//-----------------------------------------------------------------------------
//- Various helper functions
//- Dynamic dimdata update function
Adesk::Boolean CBlkJig::updateDimData () {
	//- Check the dim data store for validity
	if ( mDimData.length () <= 0 )
		return (true) ;

	/* SAMPLE CODE :
	//- Extract the individual dimData
	AcDbDimData *dimData =mDimData [m_inputNumber] ;
	//- Now get the dimension
	AcDbDimension *pDim =(AcDbDimension *)dimData->dimension () ;
	//- Check it's the type of dimension we want
	AcDbAlignedDimension *pAlnDim =AcDbAlignedDimension::cast (pDim) ;
	//- If ok
	if ( pAlnDim ) {
		//- Check to see if it is required
		if ( getDynDimensionRequired (m_inputNumber) )
			dimData->setDimInvisible (false) ;
		else
			dimData->setDimInvisible (true) ;
		pAlnDim->setXLine1Point (m_originPoint) ;
		pAlnDim->setXLine2Point (m_lastPoint) ;
		//- Get the dimPoint, first the midpoint
		AcGePoint3d dimPoint =m_originPoint + ((m_lastPoint - m_originPoint) / 2.0) ;
		//- Then the offset
		pAlnDim->setDimLinePoint (dimPoint) ;
	} */
	return (true) ;
}

//-----------------------------------------------------------------------------
//- Std input to get a point with no rubber band
AcEdJig::DragStatus CBlkJig::GetStartPoint () {
	AcGePoint3d newPnt ;
	//- Get the point 
	AcEdJig::DragStatus status =acquirePoint (newPnt) ;
	//- If valid input
	if ( status == AcEdJig::kNormal ) {
		//- If there is no difference
		if ( newPnt.isEqualTo (mInputPoints [mCurrentInputLevel]) )
			return (AcEdJig::kNoChange) ;
		//- Otherwise update the point
		mInputPoints [mCurrentInputLevel] =newPnt ;
	}
	return (status) ;
}

//-----------------------------------------------------------------------------
//- Std input to get a point with rubber band from point
AcEdJig::DragStatus CBlkJig::GetNextPoint () {
	AcGePoint3d oldPnt =mInputPoints [mCurrentInputLevel] ;
	AcGePoint3d newPnt ;
	//- Get the point 
	AcEdJig::DragStatus status =acquirePoint (newPnt, oldPnt) ;
	//- If valid input
	if ( status == AcEdJig::kNormal ) {
		//- If there is no difference
		if ( newPnt.isEqualTo (mInputPoints [mCurrentInputLevel]) )
			return (AcEdJig::kNoChange) ;
		//- Otherwise update the point
		mInputPoints [mCurrentInputLevel] =newPnt ;
	}
	return (status) ;
}


void CBlkJig::SetRotateStatus(bool bRotate)
{
	m_bRotate = bRotate;
}

void CBlkJig::SetScaleStatus(bool bRotate)
{
	m_bScale = bRotate;
}

void CBlkJig::SetBlkRefId(AcDbObjectId objId)
{
	m_blockId = objId;
}

AcDbObjectId CBlkJig::GetBlkRefId()
{
	return mpEntity->objectId();
}
