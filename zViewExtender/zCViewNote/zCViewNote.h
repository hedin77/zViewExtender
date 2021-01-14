// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  class zCViewNote : public zCView {
  protected:
    zSTRING RealText;
    zSTRING OldText;
    int Caret;

  public:
    zCViewNote();

    zSTRING Text;
    bool_t Editable;
    bool_t AutoSize;
    bool_t Multiline;

    void EditBegin();
    void EditEnd();
    void EditCancle();
    void PrintChars( int x, int y, const zSTRING& s );
    void PrintCaret( int x, int y, const zSTRING& s );
    virtual void Blit();
    virtual int HandleEvent( int );
  };
}