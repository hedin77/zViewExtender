// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  typedef bool( __cdecl* zTViewNoteEventKeyHandle )( class zCViewNote*, int& keyCode );

  class VIEWAPI zCViewNote : public zCView {
  protected:
    zSTRING RealText;
    zSTRING OldText;
    int Caret;
    int Selection;
    POINT CaretPosition;
    bool_t UpdateCaretX;
    int AutoSize;
    
  public:
    static uint DefaultCodePage;

    uint FindCaretPositionInLine( int x, int y, zCViewText* viewtext );
    void PrintChars( int x, int y, const zSTRING& s );
    void PrintCaret( int x, int y, const zSTRING& s );
    void DrawSelection( int x, int y, const zSTRING& s );
    uint GetCaretLine();
    void UpdateAutoHeight();

    void CopyText();
    void PasteText();
    void CutText();
    void DeleteSelection();
    void SelectAll();
    void SetPrevCaretPosition();
    void SetNextCaretPosition();
    void PickRange( const string& symbols );

  public:
    zCViewNote();
    zCViewNote( int a0, int a1, int a2, int a3, zTviewID a4 = zTviewID::VIEW_ITEM );
    void Reset();

    zSTRING Text;
    bool Editable;
    bool Multiline;
    bool AutoHeight;
    zTViewNoteEventKeyHandle EventKeyHandle;
    uint CursorVisualIndex;

    void EditBegin();
    void EditEnd();
    void EditCancel();
    void PickWord();
    void PickLine();
    uint GetClicksCount();
    bool SetCaretPosition( int x, int y, bool expandSelection = false, bool updateCaretX = true );
    POINT GetSelectionRange();
    zSTRING GetSelectionText();
    virtual void Blit();
    virtual int HandleEvent( int );
  };

  static zCViewNote* ActiveViewNote = Null;
}