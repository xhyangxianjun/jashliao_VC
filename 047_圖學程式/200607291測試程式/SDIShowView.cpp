// SDIShowView.cpp : implementation of the CSDIShowView class
//

#include "stdafx.h"
#include "SDIShow.h"

#include "SDIShowDoc.h"
#include "SDIShowView.h"
#include <afxtempl.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Arc.h"
#include "ChangePoint.h"
#include "Circle.h"
#include "CircleRightLeft.h"
#include "GlobalVar.h"
#include "InputData.h"
#include "Line.h"
#include "LineLine.h"
#include "NoSolution.h"
#include "Point2D.h"
#include "Point3D.h"
#include "PointArc.h"
#include "PointCircle.h"
#include "PointLine.h"
#include "RipAngle2Vector.h"
#include "RipArcCircle.h"
#include "RotateVector.h"
#include "SmoothSolution.h"
#include "Tool.h"
#include "TwoPointCircle.h"
#include "Vector.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CSDIShowView
long EngInitial(long lngBlockAmount,double tolerance,long Direction);//取得Block總數，並動態配置記憶體已用來存放每個Block的Entity數量
long EngGetValue(EngarveElement *p,long lngBlock,long lngAmount);//資料陣列指標；資料陣列屬於第幾個lngBlock；陣列大小
long EngLineLineSolve(const CInputData &BaseInputData,const CInputData &RipInputData,const CPoint3D &RipPoint3D,CCircle *AnsCircle);//線與線求切圓
long EngLineArcSolve(const CArc &BaseArc,const CLine &RipLine,const CPoint3D &RipPoint3D,CCircle *AnsCircle);//線與圓弧求切圓
long EngArcLineSolve(const CLine &BaseLine,CArc &RipArc,const CPoint3D &RipPoint3D,CCircle *AnsCircle);//圓弧與線求切圓
long EngArcArcSolve(const CArc &BaseArc,const CArc &RipArc,const CPoint3D &RipPoint3D,CCircle *AnsCircle);//圓弧與圓弧求切圓
long ArcParagraph(CArc &data,const double tolerance);//圓弧求分段點
long LineParagraph(CLine &data,const double tolerance);//線求分段點
long EngCalculate();//計算核心
long BubbleSort(double **p,long n);//二維陣列排序
long EngInitial(long lngBlockAmount,double tolerance,long Direction)//取得Block總數，並動態配置記憶體已用來存放每個Block的Entity數量
{
	long i;
	lngBlockNumber=lngBlockAmount;
	lngDirection=Direction;//0->順；1->逆
	if(lngBlockNumber<1)
		return 0;
	if(tolerance<=0)
		return 0;
	else
	{
		error=fopen("error.txt","w");
		data=fopen("data.txt","w");
		lngBlockData=new long[lngBlockNumber];
		for(i=0;i<lngBlockNumber;i++)
			lngBlockData[i]=-1;
		dblTolerance=tolerance;
		return 1;
	}
}
long EngGetValue(EngarveElement *p,long lngBlock,long lngAmount)
//資料陣列指標；資料陣列屬於第幾個lngBlock；陣列大小
{
	long i,n;
	long lngtype,lngblock;
	long return1;
	n=lngAmount;
	lngblock=lngBlock;
	try
	{
		if(lngBlock<0)
			throw 0;
		if(lngAmount<=0)
			throw 1;
	}
	catch(int err)
	{
		if(err==0)
			fprintf(error,"資料沒有分群組\n");
		else
			fprintf(error,"陣列中沒有任何資料\n");
		return 0;
	}
	lngBlockData[lngblock]=n;
	gEE=p;
	Point3DCross=new CPoint3D[n]; 
	for(i=0;i<n;i++)
	{
		lngtype=(gEE+i)->Type;
		gInputData.Reset();
		if(lngtype==1)//Line
		{
			gInputData.m_lngBlock=lngblock;//用來儲存屬於哪個Block
			gInputData.m_lngEntity=i;//用來儲存屬於哪個Entity
			gInputData.m_lngLineArc=0;//用來儲存型態0->Line；1->Arc
			gInputData.CCW=(gEE+i)->CCW;//紀錄方向，0->右；1->左
			gInputData.Direction=lngDirection;//紀錄順逆，0->順；1->逆
			gInputData.m_Line.SetData((gEE+i)->X1,(gEE+i)->Y1,(gEE+i)->Z1,(gEE+i)->X2,(gEE+i)->Y2,(gEE+i)->Z2);
			Point3DCross[i]=gInputData.m_Line.m_EndPoint;
			fprintf(data,"第 %d block 第 %d 筆 : 為線資料，其相關資料為\t%f\t%f\t%f\t%f\t%f\t%f\n",lngblock,i,gInputData.m_Line.m_StartPoint.m_x,gInputData.m_Line.m_StartPoint.m_y,gInputData.m_Line.m_StartPoint.m_z,
				gInputData.m_Line.m_EndPoint.m_x,gInputData.m_Line.m_EndPoint.m_y,gInputData.m_Line.m_EndPoint.m_z);  
		}
		else if(lngtype==2)//Arc
		{
			gInputData.m_lngBlock=lngblock;//用來儲存屬於哪個Block
			gInputData.m_lngEntity=i;//用來儲存屬於哪個Entity
			gInputData.m_lngLineArc=1;//用來儲存型態0->Line；1->Arc
			gInputData.CCW=(gEE+i)->CCW;//紀錄方向，0->右；1->左
			gInputData.Direction=lngDirection;//紀錄順逆，0->順；1->逆
			CPoint3D P3DCenter((gEE+i)->X1,(gEE+i)->Y1,(gEE+i)->Z1); 
			gInputData.m_Arc.SetData(P3DCenter,(gEE+i)->X2,(gEE+i)->Y2,(gEE+i)->Z2);
			Point3DCross[i].m_x=P3DCenter.m_x+gInputData.m_Arc.m_fltR*cos(gInputData.m_Arc.m_fltEndAngle);   
			Point3DCross[i].m_y=P3DCenter.m_y+gInputData.m_Arc.m_fltR*sin(gInputData.m_Arc.m_fltEndAngle); 
			Point3DCross[i].m_z=P3DCenter.m_z+gInputData.m_Arc.m_fltR*0; 
			fprintf(data,"第 %d block 第 %d 筆 : 為弧資料，其相關資料為\t%f\t%f\t%f\t%f\t%f\t%f\n",lngblock,i,gInputData.m_Arc.m_Point3DCenter.m_x,gInputData.m_Arc.m_Point3DCenter.m_y,gInputData.m_Arc.m_Point3DCenter.m_z,
				gInputData.m_Arc.m_fltR,gInputData.m_Arc.m_fltStartAngle,gInputData.m_Arc.m_fltEndAngle);  
		}
		gArray.Add(gInputData);
	}
	return1=(long)gArray.GetSize(); 
	return return1;
}
long EngLineLineSolve(const CInputData &BaseInputData,const CInputData &RipInputData,const CPoint3D &RipPoint3D,CCircle *AnsCircle)
{
	double ang1=0.0,ang2=0.0;
	long data1=0,data2=0;
	CCircleRightLeft CRL1,CRL2;
	CPoint3D p2;
	CCircle CC1,CC2;
	CLine Line;
	CPoint3D Point3D1,Point3D2; 
	CVector VectorBase,VectorRip;
	CLine BaseLine,RipLine;
	CPoint3D Point3DRip;
	BaseLine=BaseInputData.m_Line;
	RipLine=RipInputData.m_Line; 
	VectorBase.SetData(BaseLine.m_StartPoint,BaseLine.m_EndPoint);
	VectorRip.SetData(RipLine.m_StartPoint,RipLine.m_EndPoint);
	Point3DRip=RipPoint3D;
	CPointLine PL(VectorBase,Point3DRip,VectorRip);
	CC1.SetData(PL.GetCenter(0),PL.GetR(0));
	CC2.SetData(PL.GetCenter(1),PL.GetR(1));
	Line.SetData(BaseLine.m_StartPoint,BaseLine.m_EndPoint); 
	//data=CC1.CircleLine(Line,&ang1,&ang2,&Point3D1,&Point3D2);
	//if((data!=0) && (data!=3))
	data1=0;
	data2=0;
	if(CC1.CheckExcircle(Line)) 
	{
		data1=1;
	}
	if(CC2.CheckExcircle(Line))
	{
		data2=1;
	}
	/////////////////////////
	if((data1==1) && (data2==0))
	{
		AnsCircle->m_Point3DCenter=PL.GetCenter(0);
		AnsCircle->m_fltR=PL.GetR(0);
		return 1;
	}
	else if((data1==0) && (data2==1))
	{
		AnsCircle->m_Point3DCenter=PL.GetCenter(1);
		AnsCircle->m_fltR=PL.GetR(1);
		return 1;
	}
	else if((data1==1) && (data2==1))
	{
		CRL1.SetData(BaseInputData,CC1);
		CRL2.SetData(RipInputData,CC1);
		if(CRL1.Check() && CRL2.Check())
		{
			AnsCircle->m_Point3DCenter=PL.GetCenter(0);
			AnsCircle->m_fltR=PL.GetR(0);
			return 1;
		}
		else
		{
			CRL1.SetData(BaseInputData,CC2);
			CRL2.SetData(RipInputData,CC2);
			if(CRL1.Check() && CRL2.Check())
			{
				AnsCircle->m_Point3DCenter=PL.GetCenter(1);
				AnsCircle->m_fltR=PL.GetR(1);
				return 1;
			}
		}
	}
	else
	{
		/*
		Line.SetData(RipLine.m_StartPoint,RipLine.m_EndPoint);
		if(CC1.CheckExcircle(Line)) 
		{
			data1=1;
		}
		if(CC2.CheckExcircle(Line))
		{
			data2=1;
		}
		if((data1==1) && (data2==0))
		{
			AnsCircle->m_Point3DCenter=PL.GetCenter(0);
			AnsCircle->m_fltR=PL.GetR(0);
			return 1;
		}
		else if((data1==0) && (data2==1))
		{
			AnsCircle->m_Point3DCenter=PL.GetCenter(1);
			AnsCircle->m_fltR=PL.GetR(1);
			return 1;
		}
		*/
	}
	return 0;
}
long EngLineArcSolve(const CArc &BaseArc,const CLine &RipLine,const CPoint3D &RipPoint3D,CCircle *AnsCircle)
{

	bool bolCircle=false;
	CArc ArcBase;
	CLine LineRip;
	CPoint3D Point3DRip;
	CCircle CircleBuf[2];
	ArcBase=BaseArc;
	LineRip=RipLine;
	Point3DRip=RipPoint3D;
	CPointArc PointArc(ArcBase,LineRip,Point3DRip);
	bolCircle=PointArc.GetCircle(CircleBuf);
	int a11=ArcBase.ArcCircle(CircleBuf[0]);
	int a12=ArcBase.ArcCircle(CircleBuf[1]);

	if((a11==1) && (a12==1))
	{
		if((CircleBuf[0].m_fltR>0)&&(CircleBuf[1].m_fltR>0))
		{
			if(CircleBuf[0].m_fltR>CircleBuf[1].m_fltR)
			{
				AnsCircle->m_fltR=CircleBuf[1].m_fltR;
				AnsCircle->m_Point3DCenter=CircleBuf[1].m_Point3DCenter; 
				return 1;
			}
			else
			{
				AnsCircle->m_fltR=CircleBuf[0].m_fltR;
				AnsCircle->m_Point3DCenter=CircleBuf[0].m_Point3DCenter;
				return 1;
			}
		}
		else if(CircleBuf[0].m_fltR>0)
		{
				AnsCircle->m_fltR=CircleBuf[0].m_fltR;
				AnsCircle->m_Point3DCenter=CircleBuf[0].m_Point3DCenter;
				return 1;
		}
		else if(CircleBuf[1].m_fltR>0)
		{
				AnsCircle->m_fltR=CircleBuf[1].m_fltR;
				AnsCircle->m_Point3DCenter=CircleBuf[1].m_Point3DCenter; 
				return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(a11==1)
	{
			AnsCircle->m_fltR=CircleBuf[0].m_fltR;
			AnsCircle->m_Point3DCenter=CircleBuf[0].m_Point3DCenter;
			return 1;
	}
	else if(a12==1)
	{
			AnsCircle->m_fltR=CircleBuf[1].m_fltR;
			AnsCircle->m_Point3DCenter=CircleBuf[1].m_Point3DCenter; 
			return 1;
	}
	return 0;
}
long EngArcLineSolve(const CLine &BaseLine,CArc &RipArc,const CPoint3D &RipPoint3D,CCircle *AnsCircle)
{
	bool bolcheck1=false;
	CCircle CircleBuf;
	CRipArcCircle RAP(BaseLine,RipArc,RipPoint3D);
	bolcheck1=RAP.GetCircle(&CircleBuf);
	if(bolcheck1==true)
	{
		int a11=RipArc.ArcCircle(CircleBuf);
		if(a11==1)
		{
			if(CircleBuf.m_fltR >0)
			{
				AnsCircle->m_fltR=CircleBuf.m_fltR;
				AnsCircle->m_Point3DCenter=CircleBuf.m_Point3DCenter; 				
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
long EngArcArcSolve(const CArc &BaseArc,const CArc &RipArc,const CPoint3D &RipPoint3D,CCircle *AnsCircle)
{
	bool bolCircle=false;
	CArc ArcBase;
	CArc ArcRip;
	CPoint3D Point3DRip;
	CCircle CircleBuf[2];
	ArcBase=BaseArc;
	ArcRip=RipArc;
	Point3DRip=RipPoint3D;
	CPointArc PointArc(ArcBase,ArcRip,Point3DRip);
	bolCircle=PointArc.GetCircle(CircleBuf);
	int a11=ArcBase.ArcCircle(CircleBuf[0]);
	int a12=ArcBase.ArcCircle(CircleBuf[1]);
	if((a11==1) && (a12==1))
	{
		if((CircleBuf[0].m_fltR>0)&&(CircleBuf[1].m_fltR>0))
		{
			if(CircleBuf[0].m_fltR>CircleBuf[1].m_fltR)
			{
				AnsCircle->m_fltR=CircleBuf[1].m_fltR;
				AnsCircle->m_Point3DCenter=CircleBuf[1].m_Point3DCenter; 
				return 1;
			}
			else
			{
				AnsCircle->m_fltR=CircleBuf[0].m_fltR;
				AnsCircle->m_Point3DCenter=CircleBuf[0].m_Point3DCenter;
				return 1;
			}
		}
		else if(CircleBuf[0].m_fltR>0)
		{
				AnsCircle->m_fltR=CircleBuf[0].m_fltR;
				AnsCircle->m_Point3DCenter=CircleBuf[0].m_Point3DCenter;
				return 1;
		}
		else if(CircleBuf[1].m_fltR>0)
		{
				AnsCircle->m_fltR=CircleBuf[1].m_fltR;
				AnsCircle->m_Point3DCenter=CircleBuf[1].m_Point3DCenter; 
				return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(a11==1)
	{
			AnsCircle->m_fltR=CircleBuf[0].m_fltR;
			AnsCircle->m_Point3DCenter=CircleBuf[0].m_Point3DCenter;
			return 1;
	}
	else if(a12==1)
	{
			AnsCircle->m_fltR=CircleBuf[1].m_fltR;
			AnsCircle->m_Point3DCenter=CircleBuf[1].m_Point3DCenter; 
			return 1;
	}
	return 0;
}
long ArcParagraph(CArc &data,const double tolerance)
{
	long i,number;
	bool blnEqual=true; 
	double L,R;
	double StartAngle,EndAngle,UnitAngle;
	CPoint3D Center;
	Center=data.m_Point3DCenter;
	StartAngle=data.m_fltStartAngle;
	EndAngle=data.m_fltEndAngle;
	R=data.m_fltR; 
	L=sqrt(pow((2.0*PI*R*(EndAngle-StartAngle)/(2.0*PI)),2));
	//L=2.0*PI*R*(EndAngle-StartAngle)/(2.0*PI);
	number=(long)(L/(tolerance*10))+1;
	if(L!=(tolerance*10*number))
	{
		blnEqual=false;
	}
	if(number<=1)
		return 0;
	if(blnEqual==true)
		Point3DAns=new CPoint3D[number];
	else
		Point3DAns=new CPoint3D[number+1];
	UnitAngle=(EndAngle-StartAngle)*(tolerance*10)/L;
	for(i=0;i<number;i++)
	{
		Point3DAns[i].m_x=Center.m_x+R*cos((StartAngle+i*UnitAngle)); 
		Point3DAns[i].m_y=Center.m_y+R*sin((StartAngle+i*UnitAngle)); 
		Point3DAns[i].m_z=Center.m_z+R*0;		
		
		//Point3DAns[i].m_x=Center.m_x+R*cos(i*UnitAngle); 
		//Point3DAns[i].m_y=Center.m_y+R*sin(i*UnitAngle); 
		//Point3DAns[i].m_z=Center.m_z+R*0;	
	}
	if(blnEqual!=true)
	{
		Point3DAns[i].m_x=Center.m_x+R*cos(EndAngle);
		Point3DAns[i].m_y=Center.m_y+R*sin(EndAngle);
		Point3DAns[i].m_z=Center.m_z+0;
		number++;
	}
	return number;
}
long LineParagraph(CLine &data,const double tolerance)
{
	double L;
	long i,number;
	bool blnEqual=true; 
	CVector Vector1;
	CPoint3D UnitVector1; 
	L=data.GetDistance();
	Vector1.SetData(data.m_StartPoint,data.m_EndPoint);
	UnitVector1=Vector1.GetUnitVector(); 
	number=(long)(L/(tolerance*20))+1;
	if(L!=(tolerance*20*(number-1)))
	{
		blnEqual=false;
	}
	if(number<=1)
		return 0;
	if(blnEqual==true)
		Point3DAns=new CPoint3D[number];
	else
		Point3DAns=new CPoint3D[number+1];
	for(i=0;i<number;i++)
	{
		Point3DAns[i].m_x=data.m_StartPoint.m_x+i*(tolerance*20)*UnitVector1.m_x;
		Point3DAns[i].m_y=data.m_StartPoint.m_y+i*(tolerance*20)*UnitVector1.m_y;
		Point3DAns[i].m_z=data.m_StartPoint.m_z+i*(tolerance*20)*UnitVector1.m_z;
	}
	if(blnEqual!=true)
	{
		Point3DAns[i]=data.m_EndPoint;
		number++;
	}
	return number;
}
long EngCalculate()
{
	long lngEntityNumber=0;//存放元件數量
	long lngParagraphNumber=0;//存放分段點的數量
	long i=0,j=0,k=0;//三層迴圈的旗標變數
	long lngAnsNumber=0;//求到刀具路徑定點的數量
	long lngCheck=0;//判段是否有解
	lngEntityNumber=(long)gArray.GetSize(); //取得元件數量
	CPoint3D Point3DBuffer;//分段點暫存器
	CInputData InputDataBuffer;//輸入物件暫存器
	CInputData IDSmooth1,IDSmooth2;//為了Smooth Soluction，輸入物件暫存器
	CVector VSmooth1,VSmooth2;//為了Smooth Soluction，向量暫存器
	CVector VSmooth3,VSmooth4;//為了Smooth Soluction，向量暫存器
	CRotateVector RV;//為了產生向量旋轉的運算物件
	CSmoothSolution SS;//mooth Soluction實際計算類別
	double dblSumAngle=0.0;//存放旋轉角度
	CVector VRoate,VRoateUnit;//Smooth Soluction中產生的旋轉向量
	bool blnSmooth=false;//判斷是否要執行Smooth
	bool blnNextEntiy=false;//判斷是否在做挑角
	long lngSmoothnumber=0;//判斷執行Smooth的次數
	long lngSmoothAngle=0;//存放Smooth的角度正負號
	CPoint3D P3DUnit;//存放Smooth Soluction運算過程的單位向量
	CRipAngle2Vector RAV;//求Smooth Soluction的分段數和角度正負號
	CCircle CircleBuffer;//暫存求出來的圓形
	double **DynamicArrayR;//動態配置二維記憶體，用來儲存一個分段點所有的切圓半徑
	long l=0;//動態配置二維記憶體迴圈的旗標變數
	long m=0;//動態配置二維記憶體紀錄維度變數
	long n=0;//氣排法的回傳值，和取得排序完的結果
	CCircleRightLeft CRLBuf;//用來運算切圓是否在entity正確方向
	bool blnCRL=false;//用來儲存CRLBuf運算結果
	long lngcount=0;//用來儲存切圓與entity相交次數
	CNoSolution NSBuf;
	blnSmooth=false;
	blnNextEntiy=true;
	for(i=0;i<lngEntityNumber;i++)//(1)
	{
		gInputData.Reset();//先把暫存器資料清空
		gInputData=gArray[i];//依序取出每個entity
		if(gInputData.m_lngLineArc==0)//用來儲存型態0->Line；1->Arc)
		{
			lngParagraphNumber=LineParagraph(gInputData.m_Line,dblTolerance);//線求分段點
		}
		else
		{
			lngParagraphNumber=ArcParagraph(gInputData.m_Arc,dblTolerance);//圓弧求分段點
		}
		///////////////////////////////////////////////////////////////////
		//for(j=0;j<lngParagraphNumber;j++)//把每個分段點依序求切圓//(2)
		//取出相鄰的兩個entity
		IDSmooth1=gArray[i];
		if((i+1)!=lngEntityNumber)
			IDSmooth2=gArray[i+1];
		else
			IDSmooth2=gArray[0];
		//取出相鄰的兩個entity
		j=0;
		do//把每個分段點依序求切圓//(2)
		{
			gArrayBuffer.RemoveAll();//先把暫存一個分段點與所有entity切圓CArray清空
			Point3DBuffer=Point3DAns[j];//依序把分段點取出
			/////////////////////////////////////////////
			//判斷確認分段點是否和entity的終點相同->start
			if(blnSmooth==false&&blnNextEntiy==false)
			{
				if(IDSmooth1.m_lngLineArc==0)//LINE=0
				{
					if(fabs(IDSmooth1.m_Line.m_EndPoint.m_x-Point3DBuffer.m_x)<0.01&&
					   fabs(IDSmooth1.m_Line.m_EndPoint.m_y-Point3DBuffer.m_y)<0.01)
					{
						blnSmooth=true;
						VSmooth1.SetData(IDSmooth1.m_Line.m_StartPoint,IDSmooth1.m_Line.m_EndPoint);
						P3DUnit=VSmooth1.GetUnitVector();//取出單位向量
						P3DUnit=P3DUnit+VSmooth1.m_EndPoint;//以VSmooth1.m_EndPoint為起點求單位向量的終點
						VSmooth3.SetData(VSmooth1.m_EndPoint,P3DUnit);//產生新向量
						/////////////////////////////////////////////////
						if(IDSmooth2.m_lngLineArc==0)//LINE=0
						{
							VSmooth2.SetData(IDSmooth2.m_Line.m_StartPoint,IDSmooth2.m_Line.m_EndPoint);
							P3DUnit=VSmooth2.GetUnitVector();//取出單位向量
							P3DUnit.m_x*=-1.0;//單位向量反轉
							P3DUnit.m_y*=-1.0;//單位向量反轉
							P3DUnit.m_z*=-1.0;//單位向量反轉
							P3DUnit=P3DUnit+VSmooth2.m_StartPoint;//以VSmooth2.m_StartPoint為起點求單位向量的終點
							VSmooth4.SetData(VSmooth2.m_StartPoint,P3DUnit);//產生新向量  
						}
						else//ARC=1
						{
							long number=0;
							double L1=0.0,R=0.0;
							double StartAngle=0.0,EndAngle=0.0,UnitAngle=0.0;
							double tolerance=0.0;
							CPoint3D Center;
							CPoint3D P3DStart,P3DEnd;
							tolerance=dblTolerance;//取得弦高公差
							R=IDSmooth2.m_Arc.m_fltR;//取得entity2的半徑
							StartAngle=IDSmooth2.m_Arc.m_fltStartAngle;//取得entity2的起始角
							EndAngle=IDSmooth2.m_Arc.m_fltEndAngle;////取得entity2的終值角
							L1=sqrt(pow((2.0*PI*R*(EndAngle-StartAngle)/(2.0*PI)),2));//計算entity2的周長
							number=(long)(L1/(tolerance*10))+1;//取得entity2的分段數
							UnitAngle=(EndAngle-StartAngle)*(tolerance*10)/L1;//entity2的單位角度
							Center=IDSmooth2.m_Arc.m_Point3DCenter;//取得entity2的圓心
							P3DStart.m_x=Center.m_x+R*cos(StartAngle);//取得entity2的起始點位置 
							P3DStart.m_y=Center.m_y+R*sin(StartAngle);
							P3DStart.m_z=Center.m_z+0;
							P3DEnd.m_x=Center.m_x+R*cos(StartAngle+1*UnitAngle);//取得entity2起始點往下一分段點位置
							P3DEnd.m_y=Center.m_y+R*sin(StartAngle+1*UnitAngle);
							P3DEnd.m_z=Center.m_z+0;
							VSmooth2.SetData(P3DStart,P3DEnd);
							//////////////////////////////////
							P3DUnit=VSmooth2.GetUnitVector();//取出單位向量
							P3DUnit.m_x*=-1.0;//單位向量反轉
							P3DUnit.m_y*=-1.0;//單位向量反轉
							P3DUnit.m_z*=-1.0;//單位向量反轉
							P3DUnit=P3DUnit+VSmooth2.m_StartPoint;//以VSmooth2.m_StartPoint為起點求單位向量的終點
							VSmooth4.SetData(VSmooth2.m_StartPoint,P3DUnit);//產生新向量  
						}
					}
					else
					{
						blnSmooth=false;
					}
				}
				else//ARC=1
				{
					//計算圓弧終點
					CPoint3D P3DArcEnd;
					P3DArcEnd.m_x=IDSmooth1.m_Arc.m_Point3DCenter.m_x+IDSmooth1.m_Arc.m_fltR*cos(IDSmooth1.m_Arc.m_fltEndAngle);//計算entity1的終點位置     
					P3DArcEnd.m_y=IDSmooth1.m_Arc.m_Point3DCenter.m_y+IDSmooth1.m_Arc.m_fltR*sin(IDSmooth1.m_Arc.m_fltEndAngle);
					P3DArcEnd.m_z=IDSmooth1.m_Arc.m_Point3DCenter.m_z+0;
					//判斷圓弧終點和和分段點是否相同
					if((P3DArcEnd.m_x==Point3DBuffer.m_x) && (P3DArcEnd.m_y==Point3DBuffer.m_y))
					{
						blnSmooth=true;
						long number=0;
						double L2=0.0,R=0.0;
						double StartAngle=0.0,EndAngle=0.0,UnitAngle=0.0;
						double tolerance=0.0;
						CPoint3D Center;
						CPoint3D P3DStart,P3DEnd;
						tolerance=dblTolerance;
						R=IDSmooth1.m_Arc.m_fltR;//取得entity1的半徑
						StartAngle=IDSmooth1.m_Arc.m_fltStartAngle;//取得entity1的起始角
						EndAngle=IDSmooth1.m_Arc.m_fltEndAngle;//取得entity1的終止角
						L2=sqrt(pow((2.0*PI*R*(EndAngle-StartAngle)/(2.0*PI)),2));//計算entity1的周長
						number=(long)(L2/(tolerance*10))+1;//計算分段數
						UnitAngle=(EndAngle-StartAngle)*(tolerance*10)/L2;//計算單位角度
						Center=IDSmooth1.m_Arc.m_Point3DCenter;//取得entity1的圓心
						P3DStart.m_x=Center.m_x+R*cos(EndAngle);//計算entity1的終點位置 
						P3DStart.m_y=Center.m_y+R*sin(EndAngle);
						P3DStart.m_z=Center.m_z+0;
						P3DEnd.m_x=Center.m_x+R*cos(EndAngle-1*UnitAngle);//計算entity1終點的前一分段點位置
						P3DEnd.m_y=Center.m_y+R*sin(EndAngle-1*UnitAngle);
						P3DEnd.m_z=Center.m_z+0;
						VSmooth1.SetData(P3DEnd,P3DStart);
						P3DUnit=VSmooth1.GetUnitVector();
						P3DUnit=P3DUnit+VSmooth1.m_EndPoint;
						VSmooth3.SetData(VSmooth1.m_EndPoint,P3DUnit);  
						///////////////////////////////////////
						if(IDSmooth2.m_lngLineArc==0)//LINE=0
						{
							VSmooth2.SetData(IDSmooth2.m_Line.m_StartPoint,IDSmooth2.m_Line.m_EndPoint);
							P3DUnit=VSmooth2.GetUnitVector();
							P3DUnit.m_x*=-1.0;
							P3DUnit.m_y*=-1.0;
							P3DUnit.m_z*=-1.0;
							P3DUnit=P3DUnit+VSmooth2.m_StartPoint;
							VSmooth4.SetData(VSmooth2.m_StartPoint,P3DUnit);
						}
						else//ARC=1
						{
							tolerance=dblTolerance;//取得標準公差
							R=IDSmooth2.m_Arc.m_fltR;//取得entity2的半徑
							StartAngle=IDSmooth2.m_Arc.m_fltStartAngle;//取得entity2的起始角
							EndAngle=IDSmooth2.m_Arc.m_fltEndAngle;//取得entity2的終止角
							L2=sqrt(pow((2.0*PI*R*(EndAngle-StartAngle)/(2.0*PI)),2));//計算entity2的周長
							number=(long)(L2/(tolerance*10))+1;//計算分段數
							UnitAngle=(EndAngle-StartAngle)*(tolerance*10)/L2;//計算單位角度
							Center=IDSmooth2.m_Arc.m_Point3DCenter;//取得entity2的圓心
							P3DStart.m_x=Center.m_x+R*cos(StartAngle); //取得entity2的起始點位置
							P3DStart.m_y=Center.m_y+R*sin(StartAngle);
							P3DStart.m_z=Center.m_z+0;
							P3DEnd.m_x=Center.m_x+R*cos(StartAngle+1*UnitAngle);//取得entity2起始點的下一分段點位置
							P3DEnd.m_y=Center.m_y+R*sin(StartAngle+1*UnitAngle);
							P3DEnd.m_z=Center.m_z+0;
							VSmooth2.SetData(P3DStart,P3DEnd);
							//////////////////////////////////
							P3DUnit=VSmooth2.GetUnitVector();
							P3DUnit.m_x*=-1.0;
							P3DUnit.m_y*=-1.0;
							P3DUnit.m_z*=-1.0;
							P3DUnit=P3DUnit+VSmooth2.m_StartPoint;
							VSmooth4.SetData(VSmooth2.m_StartPoint,P3DUnit);
						}
					}
					else
					{
						blnSmooth=false;
					}
				}
			}
			//判斷確認分段點是否和entity的終點相同->end
			if((blnSmooth==true) && (lngSmoothnumber==0))
			{
				RAV.SetData(VSmooth4,VSmooth3);
				lngSmoothnumber=RAV.GetNumber();//判斷執行Smooth的次數
				lngSmoothAngle=RAV.GetUnitAngle();//存放Smooth的角度正負號
				dblSumAngle=0.0;
			}
			if((blnSmooth==true) && (lngSmoothnumber>0))
			{
					lngSmoothnumber--;
					CPoint3D P3Dbuf1;
					CPoint3D P3Dtemp(0.0f,0.0f,0.0f);
					dblSumAngle+=lngSmoothAngle;//存放旋轉角度
					RV.SetData(VSmooth4);
					VRoate=RV.Rotate(2,dblSumAngle);//Smooth Soluction中產生的旋轉向量
					P3Dbuf1=VRoate.GetUnitVector();
					P3Dtemp=P3Dtemp+Point3DBuffer;
					P3Dbuf1=P3Dbuf1+Point3DBuffer;
					VRoateUnit.SetData(P3Dtemp,P3Dbuf1); 
			}
			for(k=0;k<lngEntityNumber;k++)//依序和其他的entity作運算//(3)
			{
				if(i==k)
					continue;//同一個Entity不用算
				InputDataBuffer.Reset();//清空暫存器 
				InputDataBuffer=gArray[k];//取出相對的entity
				if(blnSmooth==false)
				{
					switch((gInputData.m_lngLineArc+InputDataBuffer.m_lngLineArc))//判萬要使用哪一個函數運算
					{
						case 0://0+0
							lngCheck=EngLineLineSolve(InputDataBuffer,gInputData,Point3DBuffer,&CircleBuffer);
							break;
						case 1://1+0,0+1
							if((gInputData.m_lngLineArc==0) && (InputDataBuffer.m_lngLineArc==1))//線與圓弧，且分段點是從線求出
							{
								lngCheck=EngLineArcSolve(InputDataBuffer.m_Arc,gInputData.m_Line,Point3DBuffer,&CircleBuffer);
							}
							else//線與圓弧，且分段點是從圓弧求出
							{
								lngCheck=EngArcLineSolve(InputDataBuffer.m_Line,gInputData.m_Arc,Point3DBuffer,&CircleBuffer);
							}
							break;
						case 2://1+1
							lngCheck=EngArcArcSolve(InputDataBuffer.m_Arc,gInputData.m_Arc,Point3DBuffer,&CircleBuffer);
							break;
					}
				}
				else//blnSmooth==true
				{
					if(InputDataBuffer.m_lngLineArc==0)
					{
						SS.SetData(VRoateUnit,InputDataBuffer.m_Line); 
					}
					else
					{
						SS.SetData(VRoateUnit,InputDataBuffer.m_Arc);
					}
					lngCheck=SS.GetAns(&CircleBuffer);
				}
				if(blnSmooth==false)
				{
				if(lngCheck!=0)//確認且圓不和其他entity相交
				{
					for(long q=0;q<lngEntityNumber;q++)
					{
						if((q!=k) && (q!=i))
						{
								InputDataBuffer.Reset();//清空暫存器 
								InputDataBuffer=gArray[q];//取出相對的entity
								if(InputDataBuffer.m_lngLineArc==0)//用來儲存型態0->Line；1->Arc)
								{//0->Line
									double angle1,angle2;
									CPoint3D P3D1,P3D2;
									int Ansnumber;
									Ansnumber=CircleBuffer.CircleLine(InputDataBuffer.m_Line,&angle1,&angle2,&P3D1,&P3D2);
									/*
									if(Ansnumber!=0)
									{
										lngCheck=0;
										break;
									}
									//*/
									///*
									if(Ansnumber==3)//線段在圓內
									{//
										lngCheck=0;
										lngcount=0;
										break;
									}
									else if(Ansnumber==2)//有兩個交點
									{
										CLine Line2d;
										Line2d.SetData(P3D1,P3D2); 
										if(Line2d.GetDistance()>0.1)//判斷兩個交點是否為同一個
										{
											lngCheck=0;
											lngcount=0;
											break;
										}
										else//兩交點為同一個
										{
											lngcount++;
											if(CircleBuffer.CheckExcircle(InputDataBuffer.m_Line)==false)
											{
												lngCheck=0;
												lngcount=0;
												break;
											}
											if(lngcount>1)
											{
												lngCheck=0;
												lngcount=0;
												break;
											}
										}
									}
									else if(Ansnumber==1)
									{
										lngcount++;
										if(CircleBuffer.CheckExcircle(InputDataBuffer.m_Line)==false)
										{
											lngCheck=0;
											lngcount=0;
											break;
										}
										if(lngcount>1)
										{
											lngCheck=0;
											lngcount=0;
											break;
										}
									}
									//*/
								}
								else
								{//1->Arc
									int Ansnumber;
									CCircle Arc2Circle;
									double angbuf1,angbuf2;
									Arc2Circle.SetData(InputDataBuffer.m_Arc.m_Point3DCenter,InputDataBuffer.m_Arc.m_fltR);   
									Ansnumber=Arc2Circle.CircleCircle(CircleBuffer,&angbuf1,&angbuf2); 
									if(Ansnumber==10 )
									{
										lngCheck=0;
										break;
									}
									else if(Ansnumber==2)
									{
										double m_fltStartAngle,m_fltEndAngle;
										bool checkang1=false,checkang2=false;
										if(angbuf1<0)
											angbuf1=angbuf1+PI*2.0;
										if(angbuf2<0)
											angbuf2=angbuf2+PI*2.0;
										m_fltStartAngle=InputDataBuffer.m_Arc.m_fltStartAngle; 
										m_fltEndAngle=InputDataBuffer.m_Arc.m_fltEndAngle; 
										if((m_fltStartAngle<m_fltEndAngle && angbuf1<=m_fltEndAngle && angbuf1>=m_fltStartAngle)
											||(m_fltStartAngle>m_fltEndAngle && angbuf1<=m_fltStartAngle && angbuf1>=m_fltEndAngle))
											checkang1=true;
										if((m_fltStartAngle<m_fltEndAngle && angbuf2<=m_fltEndAngle && angbuf2>=m_fltStartAngle)
											||(m_fltStartAngle>m_fltEndAngle && angbuf2<=m_fltStartAngle && angbuf2>=m_fltEndAngle))
											checkang2=true;
										if(checkang2==true || checkang1==true)
										{
											lngCheck=0;
											break;
										}
									}
									Ansnumber=InputDataBuffer.m_Arc.ArcCircle(CircleBuffer);
									if(Ansnumber!=0)
									{
										lngCheck=0;
										break;
									}									
								}
						}
						else
						{
								int Ansnumber;
								InputDataBuffer.Reset();//清空暫存器 
								InputDataBuffer=gArray[q];//取出相對的entity
								if(InputDataBuffer.m_lngLineArc==1)
								{
									CCircle Arc2Circle;
									double angbuf1,angbuf2;
									Arc2Circle.SetData(InputDataBuffer.m_Arc.m_Point3DCenter,InputDataBuffer.m_Arc.m_fltR);
									Ansnumber=CircleBuffer.CircleCircle(Arc2Circle,&angbuf1,&angbuf2);
									if(Ansnumber==20||Ansnumber==21)
									{
										lngCheck=0;
										break;
									}
									else if((Ansnumber==10||Ansnumber==11)&& (fabs(CircleBuffer.m_fltR-Arc2Circle.m_fltR)<0.001))
									{
										lngCheck=0;
										break;
									}
								}
								else
								{
									bool CheckEx=false;
									CheckEx=CircleBuffer.CheckExcircle(InputDataBuffer.m_Line);
									if(CheckEx==false)
									{
										lngCheck=0;
										break;
									}
								}
						}
					}
				}
				}
				///*
				if(lngCheck!=0)
				{
					InputDataBuffer.Reset();//清空暫存器 
					InputDataBuffer=gArray[i];//取出相對的entity
					blnCRL=false;//清除上一次運算結果
					CRLBuf.SetData(InputDataBuffer,CircleBuffer);
					blnCRL=CRLBuf.Check();
					if(blnCRL==false)
					{
						lngCheck=0;
					}
					else
					{
						InputDataBuffer.Reset();//清空暫存器 
						InputDataBuffer=gArray[k];//取出相對的entity
						blnCRL=false;//清除上一次運算結果
						CRLBuf.SetData(InputDataBuffer,CircleBuffer);
						blnCRL=CRLBuf.Check();
						if(blnCRL==false)
						{
							lngCheck=0;
						}
					}
				}
				//*/
				if(lngCheck!=0)//如果沒有切圓，則不用紀錄
				{
					gArrayBuffer.Add(CircleBuffer);
					///////////////////////////////////////////////////
					if(((i+1-k)==0))
						blnNextEntiy=true;//相鄰的entity
					else
						blnNextEntiy=false;//非相鄰的entity
				}
				long debug1=(long)(gArrayAns.GetSize());
			}//(3)
			m=(long)(gArrayBuffer.GetSize());//抓出一個分段點有幾切圓
			if(m<=0)
			{
				InputDataBuffer.Reset();//清空暫存器 
				InputDataBuffer=gArray[i];//取出相對的entity
				NSBuf.SetData(InputDataBuffer,i,Point3DBuffer,Point3DCross,lngEntityNumber);
				if(NSBuf.GetAns(&CircleBuffer)) 
					gArrayBuffer.Add(CircleBuffer);
			}
			m=(long)(gArrayBuffer.GetSize());//抓出一個分段點有幾切圓
			if(m>0)
			{
				DynamicArrayR=new double *[m];//動態配置記憶體
				for(l=0;l<m;l++)
					DynamicArrayR[l]=new double [2];
				for(l=0;l<m;l++)//設定動態配置記憶體的值
				{
					DynamicArrayR[l][0]=l;
					CircleBuffer=gArrayBuffer[l];
					DynamicArrayR[l][1]=CircleBuffer.m_fltR; 
				}
				n=BubbleSort(DynamicArrayR,m);//氣泡排序
				n=(long)(DynamicArrayR[0][0]);//取出最小半徑的旗標
				CircleBuffer=gArrayBuffer[n];//取出最小半徑的切圓
				gArrayAns.Add(CircleBuffer);//儲存最小半徑的切圓 
				//////////////////////////////////////////////////
				for(l=0;l<m;l++)//釋放動態二維記憶體
					delete [] DynamicArrayR[l];
				delete [] DynamicArrayR;
			}
			if(lngSmoothnumber<=0)
			{
				j++;
				blnSmooth=false;
			}
		}while(j<lngParagraphNumber);//(2)
		delete [] Point3DAns;//釋放動態一維記憶體
	}//(1)
	lngAnsNumber=gArrayAns.GetSize(); 
	delete [] Point3DCross;
	return lngAnsNumber;
}
long BubbleSort(double **p,long n)
{
	long i,j,key;
	double temp,temp1;
	if(n<=0)
		return 0;
	for(i=0;i<n;i++)
	{
		key=0;
		for(j=0;j<(n-i-1);j++)
		{
			if(p[j][1]>p[j+1][1])
			{
				temp1=p[j][1];
				p[j][1]=p[j+1][1];
				p[j+1][1]=temp1;
				///////////////////////////////
				temp=p[j][0];
				p[j][0]=p[j+1][0];
				p[j+1][0]=temp;
				key=1;
			}
		}
		if(key==0)
			break;
	}	
	return 1;
}
IMPLEMENT_DYNCREATE(CSDIShowView, CView)

BEGIN_MESSAGE_MAP(CSDIShowView, CView)
	//{{AFX_MSG_MAP(CSDIShowView)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDIShowView construction/destruction

CSDIShowView::CSDIShowView()
{
	// TODO: add construction code here

}

CSDIShowView::~CSDIShowView()
{
}

BOOL CSDIShowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSDIShowView drawing

void CSDIShowView::OnDraw(CDC* pDC)
{
	CSDIShowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	COLORREF color;

	///////////////////////////////////////////////////////////////////////////////
/*
CVector Vector1,Vector2;
CPoint3D P3DUnit; 
CRotateVector RV;
Vector1.SetData(0.0,0.0,0.0,0.0,3.0,0.0);
P3DUnit=Vector1.GetUnitVector();
RV.SetData(Vector1);
Vector2=RV.Rotate(2,-30.0);  

//*/
	///////////////////
	float L=0.0,R=0.0;
	float x1=0.0,y1=0.0,x2=0.0,y2=0.0;
	CPoint3D p1,p2;
	color=RGB(255,0,0);
	CPoint3D Point3DO(0.0f,0.0f,0.0f),Point3DA(600.0f,0.0f,0.0f),Point3DP(0.0f,0.0f,0.0f);
	CVector VectorOA(Point3DO,Point3DA);
	CPoint3D Point3DK(300.0f,-300.0f,0.0f);
	CVector Line1(Point3DK,Point3DP);
	int i;
	i=0;
	CPoint3D Point3Dtest[10];
	CClientDC dc1(this);
	CPoint PointHalfBase;//計算畫面中心為原點 
	PointHalfBase.x=PointNowBase.x/2;
	PointHalfBase.y=PointNowBase.y/2;
	dc1.MoveTo(0,PointHalfBase.y);//畫x基準線
	dc1.LineTo(PointNowBase.x,PointHalfBase.y);
	dc1.MoveTo(PointHalfBase.x,0);//畫y基準線
	dc1.LineTo(PointHalfBase.x,PointNowBase.y);
	double ang1=0.0,ang2=0.0;
	int data=0;
	CPoint3D Point3D1,Point3D2; 
	CPen pen(PS_SOLID,1,color); 
	dc1.SelectObject(&pen);
	///////////////////////////////////////////////////////
	dc1.MoveTo(PointHalfBase.x+0,PointHalfBase.y-(-100));
	dc1.LineTo(PointHalfBase.x+100,PointHalfBase.y-0);
	/////
	dc1.MoveTo(PointHalfBase.x+100,PointHalfBase.y-0);
	dc1.LineTo(PointHalfBase.x+0,PointHalfBase.y-100);
	/////
	dc1.MoveTo(PointHalfBase.x+0,PointHalfBase.y-100);
	dc1.LineTo(PointHalfBase.x+100,PointHalfBase.y-100);
	/////
	dc1.MoveTo(PointHalfBase.x+100,PointHalfBase.y-100);
	dc1.LineTo(PointHalfBase.x+200,PointHalfBase.y-200);
	/////
	dc1.MoveTo(PointHalfBase.x+200,PointHalfBase.y-200);
	dc1.LineTo(PointHalfBase.x+300,PointHalfBase.y-100);
	/////
	dc1.MoveTo(PointHalfBase.x+300,PointHalfBase.y-100);
	dc1.LineTo(PointHalfBase.x+400,PointHalfBase.y-100);
	/////
	dc1.MoveTo(PointHalfBase.x+400,PointHalfBase.y-100);
	dc1.LineTo(PointHalfBase.x+300,PointHalfBase.y-0);
	/////
	dc1.MoveTo(PointHalfBase.x+300,PointHalfBase.y-0);
	dc1.LineTo(PointHalfBase.x+400,PointHalfBase.y-(-100));
	/////
	dc1.MoveTo(PointHalfBase.x+400,PointHalfBase.y-(-100));
	dc1.LineTo(PointHalfBase.x+200,PointHalfBase.y-0);
	/////
	dc1.MoveTo(PointHalfBase.x+200,PointHalfBase.y-0);
	dc1.LineTo(PointHalfBase.x+0,PointHalfBase.y-(-100));
	///////////////////////////////////////////////////////
	dc1.MoveTo(PointHalfBase.x+Point3DO.m_x,PointHalfBase.y-Point3DO.m_y);
	dc1.LineTo(PointHalfBase.x+Point3DA.m_x,PointHalfBase.y-Point3DA.m_y);
	dc1.MoveTo(PointHalfBase.x+Line1.m_StartPoint.m_x,PointHalfBase.y-Line1.m_StartPoint.m_y);
	dc1.LineTo(PointHalfBase.x+Line1.m_EndPoint.m_x,PointHalfBase.y-Line1.m_EndPoint.m_y);
	/////////////////////
	EngarveElement L1[15];
	L1[0].Type=1;
	L1[0].CCW=0;
	L1[0].X1=100; 
	L1[0].Y1=0; 
	L1[0].Z1=0; 
	L1[0].X2=100; 
	L1[0].Y2=300; 
	L1[0].Z2=0;
	////////////////////
	L1[1].Type=1;
	L1[1].CCW=0;
	L1[1].X1=100; 
	L1[1].Y1=300; 
	L1[1].Z1=0; 
	L1[1].X2=400; 
	L1[1].Y2=300; 
	L1[1].Z2=0;
	//////////////////
	L1[2].Type=1;
	L1[2].CCW=0;
	L1[2].X1=400; 
	L1[2].Y1=300; 
	L1[2].Z1=0; 
	L1[2].X2=400; 
	L1[2].Y2=200; 
	L1[2].Z2=0;
///////////////
	L1[3].Type=1;
	L1[3].CCW=0;
	L1[3].X1=400; 
	L1[3].Y1=200; 
	L1[3].Z1=0; 
	L1[3].X2=350; 
	L1[3].Y2=150; 
	L1[3].Z2=0;
///////////////
	L1[4].Type=1;
	L1[4].CCW=0;
	L1[4].X1=350; 
	L1[4].Y1=150; 
	L1[4].Z1=0; 
	L1[4].X2=400; 
	L1[4].Y2=100; 
	L1[4].Z2=0;
///////////////
	L1[5].Type=1;
	L1[5].CCW=0;
	L1[5].X1=400; 
	L1[5].Y1=100; 
	L1[5].Z1=0; 
	L1[5].X2=400; 
	L1[5].Y2=0; 
	L1[5].Z2=0;
///////////////
	L1[6].Type=1;
	L1[6].CCW=0;
	L1[6].X1=400; 
	L1[6].Y1=0; 
	L1[6].Z1=0; 
	L1[6].X2=100; 
	L1[6].Y2=0; 
	L1[6].Z2=0;
///////////////
	L1[7].Type=1;
	L1[7].CCW=1;
	L1[7].X1=150; 
	L1[7].Y1=200; 
	L1[7].Z1=0; 
	L1[7].X2=150; 
	L1[7].Y2=250; 
	L1[7].Z2=0;
//////////////
	L1[8].Type=1;
	L1[8].CCW=1;
	L1[8].X1=150; 
	L1[8].Y1=250; 
	L1[8].Z1=0; 
	L1[8].X2=350; 
	L1[8].Y2=250; 
	L1[8].Z2=0;
/////////////
	L1[9].Type=1;
	L1[9].CCW=1;
	L1[9].X1=350; 
	L1[9].Y1=250; 
	L1[9].Z1=0; 
	L1[9].X2=350; 
	L1[9].Y2=200; 
	L1[9].Z2=0;
////////////
	L1[10].Type=1;
	L1[10].CCW=1;
	L1[10].X1=350; 
	L1[10].Y1=200; 
	L1[10].Z1=0; 
	L1[10].X2=150; 
	L1[10].Y2=200; 
	L1[10].Z2=0;
///////////////
	L1[11].Type=1;
	L1[11].CCW=1;
	L1[11].X1=150; 
	L1[11].Y1=50; 
	L1[11].Z1=0; 
	L1[11].X2=150; 
	L1[11].Y2=100; 
	L1[11].Z2=0;
//////////////
	L1[12].Type=1;
	L1[12].CCW=1;
	L1[12].X1=150; 
	L1[12].Y1=100; 
	L1[12].Z1=0; 
	L1[12].X2=350; 
	L1[12].Y2=100; 
	L1[12].Z2=0;
/////////////
	L1[13].Type=1;
	L1[13].CCW=1;
	L1[13].X1=350; 
	L1[13].Y1=100; 
	L1[13].Z1=0; 
	L1[13].X2=350; 
	L1[13].Y2=50; 
	L1[13].Z2=0;
////////////
	L1[14].Type=1;
	L1[14].CCW=1;
	L1[14].X1=350; 
	L1[14].Y1=50; 
	L1[14].Z1=0; 
	L1[14].X2=150; 
	L1[14].Y2=50; 
	L1[14].Z2=0;
	////////////////////
	long y=0;
	y=EngInitial(1,0.1,0);
	y=EngGetValue(L1,0,15);
	y=EngCalculate();
	y=y+0;
	CCircle CC1;
	
	for(i=0;i<y;i++)
	{
		CC1=gArrayAns[i];
		p2=CC1.m_Point3DCenter;
		R=CC1.m_fltR; 
		////////////////////////////////////////////////////////////////////////////////
		x1=PointHalfBase.x+p2.m_x-R; 
		y1=PointHalfBase.y-p2.m_y-R; 
		x2=PointHalfBase.x+p2.m_x+R; 
		y2=PointHalfBase.y-p2.m_y+R; 
		CPen pen(PS_SOLID,1,color); 
		dc1.SelectObject(&pen);
///*
		Sleep(100);
		dc1.Ellipse(x1,y1,x2,y2);
//*/
/*
		Sleep(100);
		if(i==0)
		{
			dc1.MoveTo(PointHalfBase.x+p2.m_x,PointHalfBase.y-p2.m_y);
		}
		else
		{
			dc1.LineTo(PointHalfBase.x+p2.m_x,PointHalfBase.y-p2.m_y);
		}
//*/
	}
	///////////////////////////////////////////////////////
	dc1.MoveTo(PointHalfBase.x+0,PointHalfBase.y-(-100));
	dc1.LineTo(PointHalfBase.x+100,PointHalfBase.y-0);
	/////
	dc1.MoveTo(PointHalfBase.x+100,PointHalfBase.y-0);
	dc1.LineTo(PointHalfBase.x+0,PointHalfBase.y-100);
	/////
	dc1.MoveTo(PointHalfBase.x+0,PointHalfBase.y-100);
	dc1.LineTo(PointHalfBase.x+100,PointHalfBase.y-100);
	/////
	dc1.MoveTo(PointHalfBase.x+100,PointHalfBase.y-100);
	dc1.LineTo(PointHalfBase.x+200,PointHalfBase.y-200);
	/////
	dc1.MoveTo(PointHalfBase.x+200,PointHalfBase.y-200);
	dc1.LineTo(PointHalfBase.x+300,PointHalfBase.y-100);
	/////
	dc1.MoveTo(PointHalfBase.x+300,PointHalfBase.y-100);
	dc1.LineTo(PointHalfBase.x+400,PointHalfBase.y-100);
	/////
	dc1.MoveTo(PointHalfBase.x+400,PointHalfBase.y-100);
	dc1.LineTo(PointHalfBase.x+300,PointHalfBase.y-0);
	/////
	dc1.MoveTo(PointHalfBase.x+300,PointHalfBase.y-0);
	dc1.LineTo(PointHalfBase.x+400,PointHalfBase.y-(-100));
	/////
	dc1.MoveTo(PointHalfBase.x+400,PointHalfBase.y-(-100));
	dc1.LineTo(PointHalfBase.x+200,PointHalfBase.y-0);
	/////
	dc1.MoveTo(PointHalfBase.x+200,PointHalfBase.y-0);
	dc1.LineTo(PointHalfBase.x+0,PointHalfBase.y-(-100));
//*/
	///////////////////////////////////////////////////////
	/////////////
	gArrayAns.RemoveAll(); 
	gArray.RemoveAll(); 

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CSDIShowView printing

BOOL CSDIShowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSDIShowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSDIShowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSDIShowView diagnostics

#ifdef _DEBUG
void CSDIShowView::AssertValid() const
{
	CView::AssertValid();
}

void CSDIShowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIShowDoc* CSDIShowView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIShowDoc)));
	return (CSDIShowDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSDIShowView message handlers

void CSDIShowView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
/*//把目前視窗大小轉換成字串記錄起來
	itoa(cx,p,10);
	itoa(cy,q,10);
	str1="X:";
	str1=str1+p;
	str1=str1+"Y:";
	str1=str1+q;
*/
	PointNowBase.x=cx;
	PointNowBase.y=cy; 
	// TODO: Add your message handler code here
	
}

void CSDIShowView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate(true);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
