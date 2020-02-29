
// TestDemoView.cpp : CTestDemoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TestDemo.h"
#endif

#include "TestDemoDoc.h"
#include "TestDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestDemoView

IMPLEMENT_DYNCREATE(CTestDemoView, CView)

BEGIN_MESSAGE_MAP(CTestDemoView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTestDemoView 构造/析构

CTestDemoView::CTestDemoView()
{
	// TODO: 在此处添加构造代码

}

CTestDemoView::~CTestDemoView()
{
}

BOOL CTestDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTestDemoView 绘制

void CTestDemoView::OnDraw(CDC* /*pDC*/)
{
	CTestDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CTestDemoView 打印

BOOL CTestDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTestDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTestDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CTestDemoView 诊断

#ifdef _DEBUG
void CTestDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CTestDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDemoDoc* CTestDemoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDemoDoc)));
	return (CTestDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestDemoView 消息处理程序
