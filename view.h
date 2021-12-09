#pragma once

#include "../../Eyser/headers/Shared.h"
#include "../../Eyser/headers/Attribute.h"
#include "../../Eyser/headers/JavaScript.h"
#include "../../Eyser/headers/Rectangle.h"

#ifdef WIN32
#include "windows.h"
#endif
#if defined(__APPLE__)&& defined(__MACH__)
#endif

namespace EyserSpace
{
        class iGraphicsContext;
    class iEvent;

        enum ParentType {eAnyParent, eView3DParent};



        enum EventEnum {eButtonDown,eButtonUp,eMouseMove,eMouseEnter,eMouseLeave,eMouseDrag,/*eDragEnter,eDragLeave,*/eDrop,eScroll,eAnimate,eRunScriptedSets,eAdjustCursor};

        class EventModifiers : public Map<String,double>

        {
        public:
                bool IsNumberOfButtonsDown(int number);
                bool AnyButtonDown();
                bool NoButtonDown();
        String ButtonsDown()
        {
            String result = "";
            for(int i=0;i<this->Size(); i++)
            {
                result += this->First(i);
            }
            return result;
        }

                // test for multiple buttons with "," - put "|" around for exclusive test
                bool IsButtonDown(String whichButton);
                void SetButtonDown(String button);
                void ClearButtonDown(String button);
       
        double TimeSinceButtonDown(String whichButton);
        double GetButtonTime(String whichButton);
       
       
        /*
        //Map<String,iEvent*> buttonUpTarget;
        iEvent* GetButtonUpTarget(const String& whichButton)
        {
            if (buttonUpTarget.Find(whichButton))
            {
                iEvent* target = buttonUpTarget[whichButton];
                buttonUpTarget.Erase(whichButton);
                return target;
            }
            return NULL;
        }
        
        void PushButtonUpTarget(const String& whichButton,iEvent* target);
        void ClearButtonUpTarget(const String& whichButton)
        {
            buttonUpTarget.Erase(whichButton);
        }
        */
        };
       
        class EventInfo
        {
        public:
                double x;
                double y;
                double oldx;
                double oldy;
        double scroll;
                double ms;
                EventEnum event;
                String eventName;
                EventModifiers* modifiers;
               
                // difference from modifier IsButtonDown = button up events signals the button as down (convenience)
                bool IsButtonDown(String whichButton);

                EventInfo()
                {
                }
               
                EventInfo(const EventInfo& cp)
                {
                        x = cp.x;
                        y = cp.y;
                        oldx = cp.oldx;
                        oldy = cp.oldy;
                        event = cp.event;
                        eventName = cp.eventName;
                        modifiers = cp.modifiers;
                }
               
#ifdef WIN32
                HWND currentWindow;
                void Capture()
                {
                        SetCapture(currentWindow);
                }
                void Release()
                {
                        ReleaseCapture();
                }
#else
                void Capture()
                {
                }
                void Release()
                {
                }
#endif

        };

   
        class iEvent : public iAttribute
        {
        public:
                virtual bool Event(EventInfo& info) {return false;};
        bool FocusKeyEvent(EventInfo& _info);
               
                virtual void    SetEventTarget(iEvent* target)
        {
            if (parent)
                if (parent->AsiEvent())
                    parent->AsiEvent()->SetEventTarget(target);
        };
                virtual iEvent* GetEventTarget()
        {
            if (parent)
                if (parent->AsiEvent())
                    return parent->AsiEvent()->GetEventTarget();
            return NULL;
        };
                void ClearEventTarget() {SetEventTarget(NULL);};

                //virtual void SetFocus() {};
                //virtual iAttribute* ClearFocus() {return NULL;};             

                virtual bool Inside(double x, double y) {return false;};
       
       
                iEvent(iAttribute* parent) : iAttribute(parent)
                {
                }


                iEvent* AsiEvent() {return this;};
       
                virtual ~iEvent()
                {
                        if (GetEventTarget() == this)
                                ClearEventTarget();
           
                }
        };
       




        class Window;


   
        class iView : public iEvent
        {
        public:
       

                iView(iAttribute* parent) : iEvent(parent)
                {
                }

                virtual ~iView();
               
                virtual void Set(const String& name, const String& value)
                {
                        iAttribute::Set(name,value);
                };

                virtual void SetRectangle(long _left, long _bottom, long _right, long _top)=0;
                virtual void GetRectangle(long &_left, long &_bottom, long &_right, long &_top)=0;
        virtual Rectangle GetRectangle()=0;
       
                void SetSize(long xsize, long ysize)
                {
                        long left, right, bottom, top;
                        GetRectangle(left,bottom,right,top);
                        SetRectangle(left,bottom,left + xsize, bottom + ysize);
                }

                virtual void SetXSize(long xsize)
                {
                        long left, right, bottom, top;
                        GetRectangle(left,bottom,right,top);
                        SetRectangle(left,bottom,left + xsize, top);
                }

                virtual void SetYSize(long ysize)
                {
                        long left, right, bottom, top;
                        GetRectangle(left,bottom,right,top);
                        SetRectangle(left,bottom,right, bottom + ysize);
                }      

                void GetSize(long &xsize, long &ysize)
                {
                        long left, right, bottom, top;
                        GetRectangle(left,bottom,right,top);
                        xsize =  right - left;
                        ysize = top - bottom;
                }

                void SetPosition(long xpos, long ypos)
                {
            SetXPosition(xpos);
            SetYPosition(ypos);
                }

                void SetXPosition(long xpos)
                {
                        long left, right, bottom, top;
                        GetRectangle(left,bottom,right,top);
                        SetRectangle(xpos,bottom,xpos + right - left, top);
                }

                void SetYPosition(long ypos)
                {
                        long left, right, bottom, top;
                        GetRectangle(left,bottom,right,top);
                        SetRectangle(left,ypos,right, ypos + top - bottom);
                }

                void GetPosition(long &xpos, long &ypos)
                {
                        long right, top;
                        GetRectangle(xpos,ypos,right,top);
                }

                long GetXPosition()
                {
                        long xpos, ypos;
                        long right, top;
                        GetRectangle(xpos,ypos,right,top);
                        return xpos;
                }

                long GetYPosition()
                {
                        long xpos, ypos;
                        long right, top;
                        GetRectangle(xpos,ypos,right,top);
                        return ypos;
                }

                long GetXSize()
                {
                        long xpos, ypos;
                        long right, top;
                        GetRectangle(xpos,ypos,right,top);
                        return right - xpos;
                }

                long GetYSize()
                {
                        long xpos, ypos;
                        long right, top;
                        GetRectangle(xpos,ypos,right,top);
                        return top - ypos;
                }

                virtual bool IsHidden()
                {
                        return false;
                }
       
                void SetMinSize(long xsize, long ysize) {SetMinXSize(xsize); SetMinYSize(ysize);};
                void GetMinSize(long &xsize, long &ysize) {xsize = GetMinXSize(); ysize = GetMinYSize();};

                virtual void SetMinXSize(long xsize)=0;
                virtual long GetMinXSize()=0;
                virtual void SetMinYSize(long ysize)=0;
                virtual long GetMinYSize()=0;

                void SetMaxSize(long xsize, long ysize) {SetMaxXSize(xsize); SetMaxYSize(ysize);};
                void GetMaxSize(long &xsize, long &ysize) {xsize = GetMaxXSize(); ysize = GetMaxYSize();};

                virtual void SetMaxXSize(long xsize)=0;
                virtual long GetMaxXSize()=0;
                virtual void SetMaxYSize(long ysize)=0;
                virtual long GetMaxYSize()=0;

                virtual void GetResizeWeight(double &xWeight, double &yWeight) {xWeight = GetXResizeWeight(); yWeight = GetYResizeWeight();}
                virtual double GetXResizeWeight() {return 1;}
                virtual double GetYResizeWeight() {return 1;}

                virtual void Resize(long xsize, long ysize) {ResizeX(xsize);ResizeY(ysize);};
                virtual void ResizeX(long xsize)=0;
                virtual void ResizeY(long ysize)=0;

                virtual void SetXOffset(long xoffset) {};
                virtual void SetYOffset(long yoffset) {};

                virtual bool canResizeX()
                {
                        return GetMaxXSize() > GetMinXSize();
                }
                virtual bool canResizeY()
                {
                        return GetMaxYSize() > GetMinYSize();
                }

                virtual void AddChild(iView* view) {};
                virtual void Draw(Rectangle clipRectangle = Rectangle())
                {
                        if (script != NULL)
                                if (script->HasFunction("draw"))
                                        script->Call("draw");
        };

                virtual void Adjust()
                {
                };

                virtual void SetDirty()
                {
                };

                virtual Window* GetWindow()
                {
                        if (parent)
                                return parent->AsiView()->GetWindow();
                        return NULL;
                }


                virtual iGraphicsContext* GetGraphicsContext()
                {
                        if (parent)
                if (parent->AsiView())
                                return parent->AsiView()->GetGraphicsContext();
                        return NULL;
                }
       
        //virtual void EraseAllChildren();
        virtual void Unregister(bool withChildren=false);
       
        iView* AsiView() {return this;};
       
        virtual bool Inside(double x, double y)
        {
            return GetRectangle().Inside((long)x,(long)y);
        };

        };
   

        class View;
       
        extern Map<View*,int> animations;
        extern bool animationActive;
               
        void StartAnimation(View* id);
        void StopAnimation(View* id);  



        enum ViewStatus {eHidden,eVisible,eInvisible};

        // implements standard view behavior
        class View : public iView
        {
        public:
                Rectangle viewRectangle;
                Rectangle clippedRectangle;

                double weightX;
                double weightY;

                long minXSize;
                long minYSize;
                long maxXSize;
                long maxYSize;

                bool minXSizeSpecified;
                bool minYSizeSpecified;
                bool maxXSizeSpecified;
                bool maxYSizeSpecified;

                long offsetX;
                long offsetY;

                ViewStatus status;

                bool calledMouseEnter;
                bool calledDragEnter;
                bool handledMouseEnter;
//              bool handledDragEnter;
                bool animating;
                double animationTime;
       
        String scriptedSets;
        String eventScriptedSets;
        String unregisterScriptedSets;
        bool hasScriptedSet;
       
        bool dirty;
        bool buttonUpTarget;
       
        Map<String,String> buttonDowns;
       
        bool IsVisible()
        {
            if (status == eVisible)
            {
                if (parent->AsView())
                    return parent->AsView()->IsVisible();
                return true;
            }
            return false;
        }
   
       
                virtual bool IsHidden()
                {
                        return status == eHidden;
                }
               
                virtual void SetDirty();

        virtual bool Inside(double x, double y)
        {
            return (x >= clippedRectangle.GetXPosition() and x <= clippedRectangle.GetXPosition() + clippedRectangle.GetXSize()) &&
            (y >= clippedRectangle.GetYPosition() and y <= clippedRectangle.GetYPosition() + clippedRectangle.GetYSize());
        };


                virtual bool Event(EventInfo& info);

       
                virtual void Adjust()
                {
                        if (script != NULL and status==eVisible)
            {
                                if (script->preAdjustFunction)
                                        script->Call("preAdjust");
                                if (script->postAdjustFunction)
                                        script->Call("postAdjust");
            }
                };
       
       
                virtual void SetRectangle(long _left, long _bottom, long _right, long _top)
                {
                        if (viewRectangle.left   != _left ||
                                viewRectangle.top    != _top ||
                                viewRectangle.right  != _right ||
                                viewRectangle.bottom != _bottom)
                        {
                SetDirty();

                                viewRectangle.left = _left;
                                viewRectangle.top = _top;
                                viewRectangle.right = _right;
                                viewRectangle.bottom = _bottom;
               
                clippedRectangle = viewRectangle;
                        }      
                }
                virtual void GetRectangle(long &_left, long &_bottom, long &_right, long &_top)
                {
                        _left = viewRectangle.left;
                        _bottom = viewRectangle.bottom;
/*                      if (status == eHidden)
                        {
                                _top = viewRectangle.bottom;
                                _right = viewRectangle.left;
                        }
                        else*/
                        {
                                _top = viewRectangle.top;
                                _right = viewRectangle.right;
                        }
                }
        virtual Rectangle GetRectangle()
        {
            return viewRectangle;
        }
                virtual void SetXSize(long xsize)
                {
                        if (GetXSize() != xsize)
                                SetDirty();
                        long left, right, bottom, top;
                        GetRectangle(left,bottom,right,top);
                        SetRectangle(left,bottom,left + xsize, top);
                }
               
                virtual void SetYSize(long ysize)
                {
                        if (GetYSize() != ysize)
                                SetDirty();
                        long left, right, bottom, top;
                        GetRectangle(left,bottom,right,top);
                        SetRectangle(left,bottom,right, bottom + ysize);
                }      
               

                virtual void SetMinXSize(long xsize)
                {
                        if (minXSize != xsize)
                                SetDirty();
                        minXSize = xsize;
                };
                virtual long GetMinXSize()
                {
                        if (status == eHidden)
                                return 0;
                        return minXSize;
                };
                virtual void SetMinYSize(long ysize)
                {
                        if (minYSize != ysize)
                                SetDirty();
                        minYSize = ysize;
                };
                virtual long GetMinYSize()
                {
                        if (status == eHidden)
                                return 0;
                        return minYSize;
                };

                virtual void SetMaxXSize(long xsize)
                {
                        if (maxXSize != xsize)
                                SetDirty();
                        maxXSize = xsize;
                };
                virtual long GetMaxXSize()
                {
                        if (status == eHidden)
                                return 10000000;
                        return maxXSize;
                };
                virtual void SetMaxYSize(long ysize)
                {
                        if (maxYSize != ysize)
                                SetDirty();
                        maxYSize = ysize;
                };
                virtual long GetMaxYSize()
                {
                        if (status == eHidden)
                                return 1000000;
                        return maxYSize;
                };
                virtual void SetXOffset(long xoffset)
                {
                        viewRectangle.left += xoffset - offsetX;
                        viewRectangle.right += xoffset - offsetX;
                        offsetX = xoffset;
            clippedRectangle = viewRectangle;
                };
                virtual void SetYOffset(long yoffset)
                {
                        viewRectangle.bottom += yoffset - offsetY;
                        viewRectangle.top += yoffset - offsetY;
                        offsetY = yoffset;
            clippedRectangle = viewRectangle;
                };
                virtual long GetXOffset()
                {
                        return offsetX;
                };
                virtual long GetYOffset()
                {
                        return offsetY;
        };
                virtual double GetXResizeWeight()
                {
                        return weightX;
                }
                virtual double GetYResizeWeight() {return weightY;}

                virtual void ResizeX(long xsize)
                {
                        if (canResizeX())
                                SetXSize(xsize);
                };
                virtual void ResizeY(long ysize)
                {
                        if (canResizeY())
                                SetYSize(ysize);
                };

                virtual bool canResizeX()
                {
                        if (status == eHidden)
                                return false;
                        return GetMaxXSize() > GetMinXSize();
                }
                virtual bool canResizeY()
                {
                        if (status == eHidden)
                                return false;
                        return GetMaxYSize() > GetMinYSize();
                }


                virtual String GetAttributeNames()  {return iAttribute::GetAttributeNames() + "|x|y|xsize|ysize|minxsize|minysize|maxxsize|maxysize|weightx|weighty|offsetx|offsety|status|resize|";};

                virtual String Get(const String& name);


                virtual void Set(const String& name, const String& value);
                virtual void ScriptedSet(const String& name, const String& value);
               
        virtual void Done(const String& type=L"");
       
                void Draw(Rectangle clipRectangle)
                {
                        dirty = false;
                        iView::Draw(clipRectangle);
                        clipRectangle.Clip(viewRectangle,offsetX,offsetY);
                        clippedRectangle = clipRectangle;
                }

        virtual iAttribute* AddChildTree(const String& xml);
       
        View* AsView() {return this;};
               
                View(iAttribute* parent) : iView(parent)
                {
                        viewRectangle.left = 0;
                        viewRectangle.right = 0;
                        viewRectangle.top = 0;
                        viewRectangle.bottom = 0;
            clippedRectangle = viewRectangle;
                        weightX = 1;
                        weightY = 1;
                        minXSize = 0;
                        minYSize = 0;
                        maxXSize = 1000000;
                        maxYSize = 1000000;
                        minXSizeSpecified = false;
                        minYSizeSpecified = false;
                        maxXSizeSpecified = false;
                        maxYSizeSpecified = false;
                        offsetX = 0;
                        offsetY = 0;
                        status = eVisible;
                        calledMouseEnter = false;
                        calledDragEnter = false;
                        handledMouseEnter = false;
//                      handledDragEnter = false;
                        animating = false;
                        dirty = true;
            hasScriptedSet = false;
            focus="##unassigned";
            buttonUpTarget = false;
                }
       
        virtual void Delete()
        {
            if (not destructed)
            {
                Unregister(true);
                if (unregisterScriptedSets.Size())
                {
                    script->Eval("me = Attribute" + script->debugNo + ";" + unregisterScriptedSets);
                }
            }
            iAttribute::Delete();
           
        }
       
        virtual ~View()
        {
           
        }

        };
   
   
   
   
        // move to own file
   
        //extern bool globalMouseExitPass;
       
        class ViewContainer : public View
        {
        public:
        enum ViewContainerType {eTopToBottom, eBottomToTop, eLeftToRight, eRightToLeft, eLayered};
        enum ViewContainerStackBehavior {eNone, eLeft, eRight, eTop, eBottom};

                ViewContainerType type;
        ViewContainerStackBehavior stacking;
       
                long childXOffset;
                long childYOffset;

//              long childXSize;
//              long childYSize;
               
                long childrenMinXSize;
                long childrenMinYSize;
                long childrenMaxXSize;
                long childrenMaxYSize;
                                       
                void SetDirty()
                {
                       
                        childrenMinXSize = -1;
                        childrenMinYSize = -1;
                        childrenMaxXSize = -1;
                        childrenMaxYSize = -1;
                       
                        View::SetDirty();
                }
               
        virtual bool IsHidden()
                {
            // A LAYERED ACTS AS BEING HIDDEN AS TO CALCULATIONS IF ALL ITS CHILDREN ARE HIDDEN
            if (type == eLayered)
            {
                bool allChildrenIsHidden = NumberOfChildren() > 0;
                for(int i=0; i<NumberOfChildren() and allChildrenIsHidden; i++)
                {
                    if (Child(i)->AsiView())
                        if (!Child(i)->AsiView()->IsHidden())
                            allChildrenIsHidden = false;
                }
                if (allChildrenIsHidden)
                    return true;
            }
           
                        return status == eHidden;
                }

       
                virtual bool BackwardLogicalDirection()
                {
                        if (type == eTopToBottom ||type == eRightToLeft)
                                return true;
                        return false;
                }
        virtual void Resize(long xsize, long ysize)
        {
                        if (not (type == eTopToBottom || type == eBottomToTop))
                ResizeY(ysize);
            ResizeX(xsize);
                        if (type == eTopToBottom || type == eBottomToTop)
                ResizeY(ysize);
        };
                virtual iAttribute* AddNewChild(const String& type);
        virtual void EraseChild(long i)
                {
                        if (i>=NumberOfChildren() or i<0)
                                return;

                        SetDirty();                    
            iAttribute::EraseChild(i);
                };

                virtual void Adjust()
                {
                        if (script != NULL and status == eVisible)
            {
                                if (script->preAdjustFunction)
                                        script->Call("preAdjust");
                                long size = NumberOfChildren();
                                for(int i=0; i<size; i++)
                    if (Child(i)->AsiView())
                        Child(i)->AsiView()->Adjust();
                                if (script->postAdjustFunction)
                                        script->Call("postAdjust");
            }
                };


                virtual void Draw(Rectangle clipRectangle = Rectangle())
                {
                        View::Draw(clipRectangle);

                        clipRectangle.Clip(viewRectangle,offsetX,offsetY);
                        clippedRectangle = clipRectangle;

                        if (clipRectangle.Visible() and status == eVisible )
                        {
                                long size = NumberOfChildren();
                                for(int i=0; i<size; i++)
                    if (Child(i)->AsiView())
                        Child(i)->AsiView()->Draw(clipRectangle);
                        }
                }

                virtual String GetAttributeNames()  {return View::GetAttributeNames() + "|type|stack|childxoffset|childyoffset|childxsize|childysize|";};

                virtual String Get(const String& name)
                {
                        if (name == "type")
                        {
                                if (type == eLayered)
                                        return "layered";
                                else
                    if (type == eRightToLeft)
                        return "righttoleft";
                    else
                        if (type == eBottomToTop)
                            return "bottomtotop";
                        else
                            return "toptobottom";
                        }
                        else
            if (name == "stack")
            {
                if (stacking == eNone)
                    return "none";
                else
                if (stacking == eLeft)
                    return "left";
                else
                if (stacking == eRight)
                    return "right";
                else
                if (stacking == eTop)
                    return "down";
                else
                    return "up";
                }
            else
                        if (name == "childxoffset")
                        {
                                return LongToString(childXOffset);
                        }
                        else
                        if (name == "childyoffset")
                        {
                                return LongToString(childYOffset);
                        }
                        else
                        if (name == "childxsize")
                        {
                                return LongToString(GetChildrenMinXSize());
                        }
                        else
                        if (name == "childysize")
                        {
                                return LongToString(GetChildrenMinYSize());
                        }
                        else
                                return View::Get(name);
                }

                virtual void Set(const String& name, const String& value);



                ViewContainer(iAttribute* parent) : View(parent)
                {
                        type = eLeftToRight;
            stacking = eNone;
                        childXOffset = 0;
                        childYOffset = 0;
                        SetDirty();
                }

                virtual ~ViewContainer()
                {
            /*
            Unregister(true);
                        EraseAllChildren();
             */
                }

                //virtual iAttribute* Find(String interfaceName);

                virtual bool Event(EventInfo& info)
                {
                        if ((status != eVisible or not clippedRectangle.Visible()))
                                if (info.event != eMouseLeave /*and info.event!= eDragLeave and info.event!= eRunScriptedSets*/)
                                {
                                        //if (GetEventTarget() == this)
                                        //      ClearEventTarget();     // FAILSAFE - SHOULD BE DONE ELSEWHERE - in setting visibility and drawing
                                        return false;
                                }
           
            bool result = false;
            // these events are offered to parents before the children
            if (info.event == eMouseEnter)
                result = View::Event(info);
               
                        if (type == eLayered)
                        {
                                for(int i=NumberOfChildren()-1; i>=0 and not destructed; i-- )
                                        if (Child(i)->AsiEvent())
                                        if (Child(i)->AsiEvent()->Event(info))
                                        {
                        if (info.event != eMouseLeave/* &&
                            info.event != eDragLeave*/)
                            return info.event != eButtonUp; // buttonup events travel all the way up to the target
                        else
                            result = true;
                    }
                        }
                        else
                        {
                                for(int i=0; i<NumberOfChildren() and not destructed; i++)
                                {
                                        if (Child(i)->AsiEvent())
                    if (Child(i)->AsiEvent()->Event(info))
                                        {
                                                result = info.event != eButtonUp; // buttonup events travel all the way up to the target
                                                if (result)
                                                        break;
                                        }
                                }
                        }
               
                        if (destructed)
                                return true;

                        // these events are offered to parent regardless if children handled them
            if (info.event == eMouseLeave)
                                return View::Event(info) || result;

                        if (result)
                                return result;

                        return View::Event(info);
                };


                virtual long GetMinXSize()
                {
                        if (status == eHidden)
                                return 0;

                        if (minXSizeSpecified) return minXSize;
                       
                        if (childrenMinXSize < 0)
                                childrenMinXSize = GetChildrenMinXSize();                              
                       
                        return childrenMinXSize;
                }

                virtual long GetChildrenMinXSize()
                {
            return GetChildrenMinXSize(0,NumberOfChildren());
        }
       
        virtual long GetChildrenMinXSize(long from, long to)
        {
                        long xsize = 0;
                        long childxsize;

            if ((type == eTopToBottom or type == eBottomToTop) and (stacking != eNone) and (to == NumberOfChildren()) and (from ==0))
                xsize = ResizeYVerticalStacking(GetYSize(),false);
            else
            {
                if (to == -1)
                    to = NumberOfChildren();
                for(int i = from; i < to; i++)
                {
                    if (Child(i)->AsiView())
                        if (!Child(i)->AsiView()->IsHidden())
                        {
                            childxsize = Child(i)->AsiView()->GetMinXSize();
                            if (stacking != eNone or type == eLayered or type == eTopToBottom or type == eBottomToTop)
                                xsize = (std::max)(xsize,childxsize);
                            else
                                xsize += childxsize;
                        }
                }
            }

                        return xsize;
                }
       
                virtual long GetMinYSize()
                {
                        if (status == eHidden)
                                return 0;

                        if (minYSizeSpecified) return minYSize;

                        if (childrenMinYSize < 0)
                                childrenMinYSize = GetChildrenMinYSize();                              
                               
                        return childrenMinYSize;
                       
                }

                virtual long GetChildrenMinYSize()
                {
            return GetChildrenMinYSize(0,NumberOfChildren());
        }
       
                virtual long GetChildrenMinYSize(long from, long to)
                {
                        long ysize = 0;
                        long childysize;


            if ((type == eLeftToRight or type == eRightToLeft) and stacking !=eNone and from == 0 and to == NumberOfChildren())
                ysize = ResizeXHorizontalStacking(GetXSize(),false);
            else
            {
                if (to == -1)
                    to = NumberOfChildren();
                for(int i = from; i < to; i++)
                {
                    if (Child(i)->AsiView())
                        if (!Child(i)->AsiView()->IsHidden())
                        {
                            childysize = Child(i)->AsiView()->GetMinYSize();
                            if (stacking != eNone or type == eLayered or type == eLeftToRight or type == eRightToLeft)
                                ysize = (std::max)(ysize,childysize);
                            else
                                ysize += childysize;
                        }
                }
            }
           
                        return ysize;
                }

                virtual long GetMaxXSize()
                {
                        if (status == eHidden)
                                return 1000000;
                       
                        if (maxXSizeSpecified) return maxXSize;

                        if (childrenMaxXSize < 0)
                                childrenMaxXSize = GetChildrenMaxXSize();                              
                       
                        return childrenMaxXSize;
                       
                }

                virtual long GetChildrenMaxXSize()
                {
            return GetChildrenMaxXSize(0,NumberOfChildren());
        }
       
                virtual long GetChildrenMaxXSize(long from, long to)
                {

                        long xsize = 0;
                        long childxsize;

                        bool yDirection = type == eTopToBottom or type == eBottomToTop;
                       
                        if (yDirection or type == eLayered)
                xsize = 1000000;

                        for(int i = from; i < to; i++)
                        {
                if (Child(i)->AsiView())
                                if (!Child(i)->AsiView()->IsHidden())
                                {
                                        childxsize = Child(i)->AsiView()->GetMaxXSize();
                                        if (yDirection or type == eLayered)
                        xsize = (std::min)(xsize,childxsize);
                                        else
                                                xsize += childxsize;
                                }
                        }
                       
                        if (xsize < GetMinXSize())      // minimum wins TODO: optimize
                                xsize = GetMinXSize();
                       
                        return xsize;
                }

                virtual long GetMaxYSize()
                {
                        if (status == eHidden)
                                return 1000000;

                        if (maxYSizeSpecified) return maxYSize;

                        if (childrenMaxYSize < 0)
                                childrenMaxYSize = GetChildrenMaxYSize();                              
                       
                        return childrenMaxYSize;
                }

                virtual long GetChildrenMaxYSize()
        {
            return GetChildrenMaxYSize(0,NumberOfChildren());
        }
       
                virtual long GetChildrenMaxYSize(long from, long to)
                {
                        long ysize = 0;
                        long childysize;
           
                        bool yDirection = (type == eTopToBottom) or (type == eBottomToTop);
                       
            if (!yDirection or type == eLayered)
                                ysize = 1000000;

                        for(int i = from; i < to; i++)
                        {
                if (Child(i)->AsiView())
                                if (!Child(i)->AsiView()->IsHidden())
                                {
                                        childysize = Child(i)->AsiView()->GetMaxYSize();
                                        if (yDirection and type != eLayered)
                                                ysize += childysize;
                                        else
                        ysize = (std::min)(ysize,childysize);
                                }
                        }
                       
                        if (ysize < GetMinYSize())      // minimum wins TODO: optimize
                                ysize = GetMinYSize();
                       
                        return ysize;
                }

        /// ALGORITM:
       
        //// REFACOR GETMIN/MAX TO FIND MIN MAX SIZE OF WRAPPING CONTAINER
        //// MIN: EITHER A SET FACTOR OR THIS BIGGEST OF GETMIN
        //// MAX: WITDH OF CONTAINER WHEN THE FOUND MIN IS SET
        //// RESIZE: START BY ADDING MIN VALUES UNTIL > XSIZE
        ////         START OVER AND CREATE A Deque WITH NUMBER OF CHILDREN PR COLUMN
        ////         RESIZE PR. COLUMN
       
       
        /// SPLIT RESIZERS INTO DIFFERENT FUNCTIONS - BASED ON TYPE

                virtual void ResizeX(long xsize)
                {
                        long oldXSize = GetXSize();
                        if (status == eHidden)
                                return;
           
                        long minXSize = GetMinXSize();
            long maxXSize = GetMaxXSize();
                       
            if (xsize < minXSize)
                                xsize = minXSize;
                        if (xsize > maxXSize)
                                xsize = maxXSize;

                        SetXSize(xsize);
           
                        if (type == eLeftToRight or type == eRightToLeft)
            {
                if (stacking != eNone)
                    ResizeXHorizontalStacking(xsize);
                else
                    ResizeXHorizontal(xsize);
                        }
            else
            if (stacking == eNone) // ONLY RESIZE Y IF TOPTOBOTTOM IS NOT STACKING
                ResizeXVertical(xsize);
           
                        if (script != NULL)
                                if (oldXSize != GetXSize())
                                {
                                        if (script->HasFunction("resizeX"))
                                                script->Call("resizeX");
                                }
                }
       
        virtual long ResizeXHorizontalStacking(long xsize, bool doResize = true)
        {
            if (!NumberOfChildren() or xsize == 0)
                return GetYSize();
            if (doResize)
                SetXSize(xsize);

            Deque<long> childStacking;
            long xcount = xsize;
            int from = 0;
            int to = NumberOfChildren()-1;
            int it = 1;
            if (type == eRightToLeft)
            {
                (std::swap)(from,to);
                it = -1;
            }
            to +=it;
            int count = 0;
            for(int i=from; i!= to; i+=it)
            {
                if (Child(i)->AsiView())
                    if (!Child(i)->AsiView()->IsHidden())
                    {
                        if (xcount < Child(i)->AsiView()->GetMinXSize())
                        {
                            xcount = xsize;
                            if (count != 0)
                            {
                                if (it == -1)
                                    childStacking.PushFront(i+1);
                                else
                                    childStacking.Push(i);
                            }
                            else
                            {
                                childStacking.Push(-1);
                                i = to - it;
                            }
                            count = 0;
                        }
                        xcount -= Child(i)->AsiView()->GetMinXSize();
                        count++;
                    }
            }
           
            if (childStacking.Size())
            {
                if (childStacking.Back() != NumberOfChildren())
                    childStacking.Push(NumberOfChildren());
            }
            else
                childStacking.Push(-1);
           
            int pos=0;
            from=0;
           
            if (stacking == eTop)
                it = 1;
            else
                it = -1;
           
            Deque<int> newPositions;
            Deque<int> newSizes;
            for(int i=0; i<childStacking.Size(); i++)
            {
                int to = childStacking[i];
                long ysize = GetChildrenMinYSize(from,to);
                if (to == -1)
                    to = NumberOfChildren();
                if (it == -1)
                    newPositions.Push(-pos-ysize);
                else
                    newPositions.Push(pos);
                newSizes.Push(ysize);
                pos += ysize;
                from = to;
            }
            if (doResize)
            {
                from =0;
                for(int i=0; i<childStacking.Size(); i++)
                {
                    int to = childStacking[i];
                    if (to == -1)
                        to = NumberOfChildren();
                    if (it == -1)
                        ResizeYHorizontal(newSizes[i],from,to, pos+newPositions[i]);
                    else
                        ResizeYHorizontal(newSizes[i],from,to, newPositions[i]);
                    ResizeXHorizontal(xsize,from,to);
                    from = to;
                }
                SetYSize(pos);
            }
            return pos;
        }

        virtual void ResizeXHorizontal(long xsize, long from=0, long to=-1)
        {
            if (to == -1)
                to = NumberOfChildren();
           
            if (from == 0 and to == NumberOfChildren())
                SetXSize(xsize);
           
            long position = GetXPosition() + childXOffset + offsetX;
            long originalsize = xsize;
           
           
            xsize -= GetChildrenMinXSize(from,to);
           
            Deque<bool> resizers;
            for(int i=0; i<from; i++)
                resizers.Push(false);
            for(int i=from; i<to; i++)
                if (Child(i)->AsiView())
                    resizers.Push(Child(i)->AsiView()->canResizeX());
                else
                    resizers.Push(false);
           
            // RESIZE is recursive
            double weightSum = 0;
            bool rerun = true;
            while (rerun)
            {
                rerun = false;
               
                // find added weights
                weightSum = 0;
                for(int i=from; i<to; i++)
                    if (Child(i)->AsiView())
                                                if (!Child(i)->AsiView()->IsHidden())
                                                {
                                                        if (resizers[i])
                                                                weightSum += Child(i)->AsiView()->GetXResizeWeight();
                                                }
               
                // check for any resizers hitting their max size!!! (then they become fixed)
                for(int i=from; i<to; i++)
                    if (Child(i)->AsiView())
                                                if (!Child(i)->AsiView()->IsHidden())
                                                {
                                                        if (resizers[i])
                                                        {
                                                                double newSize = Child(i)->AsiView()->GetMinXSize() + ((double) xsize) / weightSum *  Child(i)->AsiView()->GetXResizeWeight();
                                                                if (newSize > Child(i)->AsiView()->GetMaxXSize())
                                                                {
                                                                        resizers.At(i) = false;
                                                                        xsize -= (Child(i)->AsiView()->GetMaxXSize() - Child(i)->AsiView()->GetMinXSize());
                                                                        rerun = true;
                                                                }
                                                        }
                                                }
               
                if (not rerun)
                {
                    long measure = 0;
                    for(int i=from; i<to; i++)
                        if (Child(i)->AsiView())
                            if (!Child(i)->AsiView()->IsHidden())
                                                        {
                                                                double newSize;
                                                                if (resizers[i])        // real resizer
                                                                        newSize = Child(i)->AsiView()->GetMinXSize() + ((double) xsize) / weightSum *  Child(i)->AsiView()->GetXResizeWeight();
                                                                else
                                    if (Child(i)->AsiView()->canResizeX()) // resizer maxed out
                                        newSize = Child(i)->AsiView()->GetMaxXSize();
                                    else
                                        newSize = Child(i)->AsiView()->GetMinXSize(); // non resizable
                                bool remDirty = dirty;
                                                                Child(i)->AsiView()->ResizeX((long)newSize);
                                dirty = remDirty;
                                                                measure += (long)newSize;
                                                        }

                    if (type == eRightToLeft)
                        position += (originalsize - measure);
                   
                    for(int i=from; i<to; i++)
                        if (Child(i)->AsiView())
                            if (!Child(i)->AsiView()->IsHidden())
                                                        {
                                                                Child(i)->AsiView()->SetXPosition(position);
                                                                position += Child(i)->AsiView()->GetXSize();
                                                        }
                }
            }
           
        }
       
        virtual void ResizeXVertical(long xsize, long from=0, long to=-1, long extraOffset = 0)
        {
                        long position = GetXPosition() + childXOffset + offsetX + extraOffset;
            if (to == -1)
                to = NumberOfChildren();
           
            if (from == 0 and to == NumberOfChildren())
                SetXSize(xsize);
           
            for(int i=from; i<to; i++)
                if (Child(i)->AsiView())
                                        //if (!Child(i)->AsiView()->IsHidden())
                                        {
                        Child(i)->AsiView()->SetXPosition(position);
                                                Child(i)->AsiView()->ResizeX(xsize);
                                        }
        }

                virtual void ResizeY(long ysize)
                {
                        long oldYSize = GetYSize();
                        if (status == eHidden)
                                return;

                        long minYSize = GetMinYSize();
                        long maxYSize = GetMaxYSize();

                        if (ysize < minYSize)
                                ysize = minYSize;
                        if (ysize > maxYSize)
                                ysize = maxYSize;


                        SetYSize(ysize);

                        if (type == eTopToBottom or type == eBottomToTop)
            {
                if (stacking != eNone)
                    ResizeYVerticalStacking(ysize);
                else
                    ResizeYVertical(ysize);
            }
            else
            if (stacking == eNone) // ONLY RESIZE Y IF LEFTTORIGHT IS NOT STACKING
                ResizeYHorizontal(ysize);
           
            if (script != NULL)
                                if (oldYSize != GetYSize())
                                {
                                        if (script->HasFunction("resizeY"))
                                                script->Call("resizeY");
                                }
                }

        virtual long ResizeYVerticalStacking(long ysize, bool doResize = true)
        {
            if (!NumberOfChildren() or ysize == 0)
                return GetXSize();
            if (doResize)
                SetYSize(ysize);
            Vector<long> childStacking;
            long ycount = ysize;
            int from = 0;
            int to = NumberOfChildren()-1;
            int it = 1;
            if (type == eTopToBottom)
            {
                (std::swap)(from,to);
                it = -1;
            }
            to +=it;
            int count = 0;
            for(int i=from; i!= to; i+=it)
            {
                if (Child(i)->AsiView())
                    if (!Child(i)->AsiView()->IsHidden())
                    {
                        if (ycount < Child(i)->AsiView()->GetMinYSize())
                        {
                            ycount = ysize;
                            if (count != 0) // ysize is smaller than one of the children - special case to avoid infinite loop
                            {
                                if (it == -1)
                                    childStacking.PushFront(i+1);
                                else
                                    childStacking.Push(i);
                            }
                            else
                            {
                                childStacking.Push(-1);
                                i = to-it;
                            }
                            count = -1;
                        }
                        ycount -= Child(i)->AsiView()->GetMinYSize();
                        count++;
                    }
            }
           
            if (childStacking.Size())
            {
                if (childStacking.Back() != NumberOfChildren() and childStacking.Back() != -1)
                    childStacking.Push(NumberOfChildren());
            }
            else
                childStacking.Push(-1);

            int pos=0;
            from=0;
           
            if (stacking == eLeft)
                it = 1;
            else
                it = -1;
           
            Deque<int> newPositions;
            Deque<int> newSizes;
            for(int i=0; i<childStacking.Size(); i++)
            {
                int to = childStacking[i];
                long xsize = GetChildrenMinXSize(from,to);
                if (to == -1)
                    to = NumberOfChildren();
                if (it == -1)
                    newPositions.Push(-pos-xsize);
                else
                    newPositions.Push(pos);
                newSizes.Push(xsize);
                pos += xsize;
                from = to;
            }
            if (doResize)
            {
                from =0;
                for(int i=0; i<childStacking.Size(); i++)
                {
                    int to = childStacking[i];
                    if (to == -1)
                        to = NumberOfChildren();
                    if (it == -1)
                        ResizeXVertical(newSizes[i],from,to, pos+newPositions[i]);
                    else
                         ResizeXVertical(newSizes[i],from,to, newPositions[i]);
                    ResizeYVertical(ysize,from,to);
                    from = to;
                }
                SetXSize(pos);
            }
            return pos;
        }
       
        virtual void ResizeYVertical(long ysize, long from=0, long to=-1)
        {
            if (to == -1)
                to = NumberOfChildren();

            if (from == 0 and to == NumberOfChildren())
                SetYSize(ysize);

            long position = GetYPosition() + childYOffset + offsetY;
            long originalysize = ysize;
           
            ysize -= GetChildrenMinYSize(from,to);

            Deque<bool> resizers;
            for(int i=0; i<from; i++)
                resizers.Push(false);
            for(int i=from; i<to; i++)
                if (Child(i)->AsiView())
                    resizers.Push(Child(i)->AsiView()->canResizeY());
                else
                    resizers.Push(false);
                       
            // RESIZE is recursive
            double weightSum = 0;
            bool rerun = true;
            while (rerun)
            {
                rerun = false;
               
                // find added weights
                weightSum = 0;
                for(int i=from; i<to; i++)
                    if (Child(i)->AsiView())
                        if (!Child(i)->AsiView()->IsHidden())
                            if (resizers[i])
                                weightSum += Child(i)->AsiView()->GetYResizeWeight();
               
                // check for any resizers hitting their max size!!! (then they become fixed)
                for(int i=from; i<to; i++)
                    if (Child(i)->AsiView())
                        if (!Child(i)->AsiView()->IsHidden())
                        {
                            if (resizers[i])
                            {
                                double newSize = Child(i)->AsiView()->GetMinYSize() + ((double) ysize) / weightSum *  Child(i)->AsiView()->GetYResizeWeight();
                                if (newSize > Child(i)->AsiView()->GetMaxYSize())
                                {
                                    resizers.At(i) = false;
                                    ysize -= (Child(i)->AsiView()->GetMaxYSize() - Child(i)->AsiView()->GetMinYSize());
                                    rerun = true;
                                }
                            }
                        }
               
                if (not rerun)
                {
                    // FIRST RUN TO SET SIZE AND SUM IT
                    long measure = 0;
                    for(int i=from; i<to; i++)
                        if (Child(i)->AsiView())
                            if (!Child(i)->AsiView()->IsHidden())
                            {
                                double newSize;
                                if (resizers[i])        // real resizer
                                    newSize = Child(i)->AsiView()->GetMinYSize() + ((double) ysize) / weightSum *  Child(i)->AsiView()->GetYResizeWeight();
                                else
                                    if (Child(i)->AsiView()->canResizeY()) // resizer maxed out
                                        newSize = Child(i)->AsiView()->GetMaxYSize();
                                    else
                                        newSize = Child(i)->AsiView()->GetMinYSize(); // non resizable
                                Child(i)->AsiView()->ResizeY((long)newSize);
                                measure += (long)newSize;
                            }
                   
                    if (type == eTopToBottom)
                        position += (originalysize - measure);

                    //SECOND RUN TO SET POSITIONS
                    for(int i=from; i<to; i++)
                        if (Child(i)->AsiView())
                            if (!Child(i)->AsiView()->IsHidden())
                            {
                                Child(i)->AsiView()->SetYPosition(position);
                                position += Child(i)->AsiView()->GetYSize();
   
                            }
                }
            }
        }

        virtual void ResizeYHorizontal(long ysize, long from=0, long to=-1,long extraOffset = 0)
        {
            long position = GetYPosition() + childYOffset + offsetY + extraOffset;
            if (to == -1)
                to = NumberOfChildren();

            if (from == 0 and to == NumberOfChildren())
                SetYSize(ysize);
           
            for(int i=from; i<to; i++)
                if (Child(i)->AsiView())
                    if (!Child(i)->AsiView()->IsHidden())
                    {
                        Child(i)->AsiView()->SetYPosition(position);
                        Child(i)->AsiView()->ResizeY(ysize);
                    }
        }

        };


        class ViewFiller : public View
        {
        public:
                ViewFiller(iAttribute* parent) : View(parent)
                {
                }

                virtual void AddChild(iView* view) {};
                virtual void Draw(Rectangle clipRectangle = Rectangle()) {View::Draw(clipRectangle);};

        };

   




        class iVisitor;
        class iVisit
        {
        public:
                virtual void Visit(iVisitor* visitor) {};
                virtual void VisitChildren(iVisitor* visitor) {};
        };
       
        class iVisitor
        {
        public:
                virtual void Visit(iVisit* visitee) { }
                virtual void StartVisit(iVisit* visitee) {};
        };

}
