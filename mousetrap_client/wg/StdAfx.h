#include <afxwin.h>   // MFC core and standard components
#include <afxext.h>   // MFC extensions
#include <afxdisp.h>  // MFC Automation extensions
#include <afxwin.h>
class CMyApp : public CWinApp
      {
      public:
         virtual BOOL InitInstance();
         virtual int ExitInstance();
      protected:
      BOOL m_bRun;
};