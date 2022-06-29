/*
 * File: gevents.h
 * ---------------
 * This file defines the event types used in the StanfordCPPLib
 * graphics libraries.  The structure of this package is adapted from
 * the Java event model.
 * <include src="pictures/ClassHierarchies/GEventHierarchy-h.html">
 * 
 * @version 2018/07/16
 * - added scroll events
 * @version 2018/06/24
 * - added hyperlink events
 * @version 2018/06/23
 * - added change events
 * - ANY_EVENT bug fix
 * @version 2018/06/20
 * - added mouse entered, exit, wheel events
 * @version 2016/11/26
 * - added WINDOW_CLOSING event
 * - added isCtrlOrCommandKeyDown
 * @version 2016/11/20
 * - added operator << for all event types
 * - added TABLE_EDIT_BEGIN and TABLE_REPLACE_BEGIN events
 * @version 2016/10/16
 * - added GEvent.isShiftKeyDown
 * - alphabetized methods
 * @version 2015/11/07
 * - added GTable TABLE_EVENT and TABLE_UPDATED
 */

#ifndef _gevents_h
#define _gevents_h

#include <string>
#include "gtable.h"
#include "gtimer.h"
#include "gwindow.h"

/*
 * Type: EventClassType
 * --------------------
 * This enumeration type defines the event classes.  The element values
 * are each a single bit and can be added or ORed together to generate
 * an event mask.  The <code>CLICK_EVENT</code> class responds only to
 * the MOUSE_CLICKED event type.  The <code>ANY_EVENT</code> class
 * selects any event.
 */
enum EventClassType {
    NULL_EVENT      = 0x0000,
    ACTION_EVENT    = 0x0010,
    KEY_EVENT       = 0x0020,
    TIMER_EVENT     = 0x0040,
    WINDOW_EVENT    = 0x0080,
    MOUSE_EVENT     = 0x0100,
    CLICK_EVENT     = 0x0200,
    TABLE_EVENT     = 0x0400,
    SERVER_EVENT    = 0x0800,
    CHANGE_EVENT    = 0x1000,
    HYPERLINK_EVENT = 0x2000,
    SCROLL_EVENT    = 0x4000,

    // ANY_EVENT should be the logical OR of all other event types,
    // to indicate that all/any events should be matched
    ANY_EVENT    = ACTION_EVENT
                 | KEY_EVENT
                 | TIMER_EVENT
                 | WINDOW_EVENT
                 | MOUSE_EVENT
                 | CLICK_EVENT
                 | TABLE_EVENT
                 | SERVER_EVENT
                 | CHANGE_EVENT
                 | HYPERLINK_EVENT
                 | SCROLL_EVENT
};

/*
 * Type: EventType
 * ---------------
 * This enumeration type defines the event types for all events.
 */
typedef enum {
    WINDOW_CLOSED       = WINDOW_EVENT + 1,
    WINDOW_RESIZED      = WINDOW_EVENT + 2,
    CONSOLE_CLOSED      = WINDOW_EVENT + 3,
    WINDOW_CLOSING      = WINDOW_EVENT + 4,

    ACTION_PERFORMED    = ACTION_EVENT + 1,

    MOUSE_CLICKED       = MOUSE_EVENT + 1,
    MOUSE_PRESSED       = MOUSE_EVENT + 2,
    MOUSE_RELEASED      = MOUSE_EVENT + 3,
    MOUSE_MOVED         = MOUSE_EVENT + 4,
    MOUSE_DRAGGED       = MOUSE_EVENT + 5,
    MOUSE_ENTERED       = MOUSE_EVENT + 6,
    MOUSE_EXITED        = MOUSE_EVENT + 7,
    MOUSE_WHEEL_DOWN    = MOUSE_EVENT + 8,
    MOUSE_WHEEL_UP      = MOUSE_EVENT + 9,

    KEY_PRESSED         = KEY_EVENT + 1,
    KEY_RELEASED        = KEY_EVENT + 2,
    KEY_TYPED           = KEY_EVENT + 3,

    TIMER_TICKED        = TIMER_EVENT + 1,

    TABLE_UPDATED       = TABLE_EVENT + 1,
    TABLE_SELECTED      = TABLE_EVENT + 2,
    TABLE_EDIT_BEGIN    = TABLE_EVENT + 3,
    TABLE_REPLACE_BEGIN = TABLE_EVENT + 4,   // like an edit but wipes out previous value
    TABLE_CUT           = TABLE_EVENT + 5,   // clipboard stuff
    TABLE_COPY          = TABLE_EVENT + 6,
    TABLE_PASTE         = TABLE_EVENT + 7,

    SERVER_REQUEST      = SERVER_EVENT + 1,

    STATE_CHANGED       = CHANGE_EVENT + 1,

    HYPERLINK_CLICKED   = HYPERLINK_EVENT + 1,
    SCROLL_PERFORMED    = SCROLL_EVENT + 1
} EventType;

/*
 * Type: ModifierCodes
 * -------------------
 * This enumeration type defines a set of constants used to check whether
 * modifiers are in effect.
 */
enum ModifierCodes {
    SHIFT_DOWN     = 1 << 0,
    CTRL_DOWN      = 1 << 1,
    META_DOWN      = 1 << 2,
    ALT_DOWN       = 1 << 3,
    ALT_GRAPH_DOWN = 1 << 4,
    BUTTON1_DOWN   = 1 << 5,
    BUTTON2_DOWN   = 1 << 6,
    BUTTON3_DOWN   = 1 << 7
};

/*
 * Type: KeyCodes
 * --------------
 * This type defines the names of the key codes returned in a key event.
 */
enum KeyCodes {
    BACKSPACE_KEY = 8,
    TAB_KEY = 9,
    ENTER_KEY = 10,
    CLEAR_KEY = 12,
    ESCAPE_KEY = 27,
    PAGE_UP_KEY = 33,
    PAGE_DOWN_KEY = 34,
    END_KEY = 35,
    HOME_KEY = 36,
    LEFT_ARROW_KEY = 37,
    UP_ARROW_KEY = 38,
    RIGHT_ARROW_KEY = 39,
    DOWN_ARROW_KEY = 40,
    F1_KEY = 112,
    F2_KEY = 113,
    F3_KEY = 114,
    F4_KEY = 115,
    F5_KEY = 116,
    F6_KEY = 117,
    F7_KEY = 118,
    F8_KEY = 119,
    F9_KEY = 120,
    F10_KEY = 121,
    F11_KEY = 122,
    F12_KEY = 123,
    DELETE_KEY = 127,
    HELP_KEY = 156
};

/* Forward definitions */
class GActionEvent;
class GChangeEvent;
class GHyperlinkEvent;
class GKeyEvent;
class GMouseEvent;
class GObject;
class GScrollEvent;
class GServerEvent;
class GTableEvent;
class GTimerEvent;
class GWindowEvent;

/*
 * Class: GEvent
 * -------------
 * This class is the root of the hierarchy for all events.
 * <include src="pictures/ClassHierarchies/GEventHierarchy.html">
 *
 * <p>The standard paradigm for using <code>GEvent</code> is illustrated
 * by the following program, which allows the user to draw lines on the
 * graphics window:
 *
 *<pre>
 *    int main() {
 *       GWindow gw;
 *       GLine *line;
 *       cout << "This program lets the user draw lines by dragging." << endl;
 *       while (true) {
 *          GMouseEvent e = waitForEvent(MOUSE_EVENT);
 *          if (e.getEventType() == MOUSE_PRESSED) {
 *             line = new GLine(e.getX(), e.getY(), e.getX(), e.getY());
 *             gw.add(line);
 *          } else if (e.getEventType() == MOUSE_DRAGGED) {
 *             line->setEndPoint(e.getX(), e.getY());
 *          }
 *       }
 *    }
 *</pre>
 */
class GEvent {
public:

    /*
     * Friend constructor: GEvent
     * Usage: GEvent event;
     * --------------------
     * Ensures that an event is properly initialized to a null/empty event.
     */
    GEvent();

    /*
     * Method: getEventClass
     * Usage: EventClassType eventClass = e.getEventClass();
     * -----------------------------------------------------
     * Returns the enumerated type constant indicating the class of the
     * event, such as WINDOW_EVENT or ACTION_EVENT.
     */
    EventClassType getEventClass() const;

    /*
     * Method: getEventTime
     * Usage: double time = e.getEventTime();
     * --------------------------------------
     * Returns the system time in milliseconds at which the event occurred.
     * To ensure portability among systems that represent time in different
     * ways, the StanfordCPPLib packages use type <code>double</code> to
     * represent time, which is always encoded as the number of milliseconds
     * that have elapsed since 00:00:00 UTC on January 1, 1970, which is
     * the conventional zero point for computer-based time systems.
     */
    double getEventTime() const;

    /*
     * Method: getEventType
     * Usage: EventType type = e.getEventType();
     * -----------------------------------------
     * Returns the enumerated type constant corresponding to the specific
     * event type.  The event type is a subcategory within the event class,
     * such as MOUSE_RELEASED within the MOUSE_EVENT class or WINDOW_CLOSED
     * within the WINDOW_EVENT class.
     */
    EventType getEventType() const;

    /*
     * Method: getModifiers
     * Usage: int modifiers = e.getModifiers();
     * ----------------------------------------
     * Returns an integer whose bits indicate what modifiers are in effect.
     * To check whether the shift key is down, for example, one could use
     * the following code:
     *
     *<pre>
     *    if (e.getModifiers() & SHIFT_DOWN) ...
     *</pre>
     */
    int getModifiers() const;

    /*
     * Method: isAltKeyDown
     * Usage: if (e.isAltKeyDown()) ...
     * --------------------------------
     * Returns <code>true</code> if the Alt key was held down during this event.
     */
    bool isAltKeyDown() const;

    /*
     * Method: isCtrlKeyDown
     * Usage: if (e.isCtrlKeyDown()) ...
     * ---------------------------------
     * Returns <code>true</code> if the Ctrl key was held down during this event.
     */
    bool isCtrlKeyDown() const;

    /*
     * Method: isCtrlOrCommandKeyDown
     * Usage: if (e.isCtrlOrCommandKeyDown()) ...
     * ---------------------------------
     * Returns <code>true</code> if the Ctrl key, or the Command key (Mac),
     * was held down during this event.
     */
    bool isCtrlOrCommandKeyDown() const;

    /*
     * Method: isMetaKeyDown
     * Usage: if (e.isMetaKeyDown()) ...
     * ---------------------------------
     * Returns <code>true</code> if the Meta/Command key was held down during this event.
     */
    bool isMetaKeyDown() const;

    /*
     * Method: isShiftKeyDown
     * Usage: if (e.isShiftKeyDown()) ...
     * ----------------------------------
     * Returns <code>true</code> if the Shift key was held down during this event.
     */
    bool isShiftKeyDown() const;

    /*
     * Method: isValid
     * Usage: if (e.isValid()) ...
     * ---------------------------
     * Returns <code>true</code> if the event is valid.
     */
    bool isValid();

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    virtual std::string toString() const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Method: setEventTime
     * Usage: e.setEventTime(time);
     * ----------------------------
     * Sets the event time field for this event.  The event system needs
     * access to this method, but conventional clients don't.
     */
    void setEventTime(double time);

    /*
     * Method: setModifiers
     * Usage: e.setModifiers(modifiers);
     * ---------------------------------
     * Sets the modifiers field for this event.  The event system needs
     * access to this method, but conventional clients don't.
     */
    void setModifiers(int modifiers);

private:
    /*
     * Instance variables
     * ------------------
     * Implementation note: All the variables from the subclasses are included
     * in the outer class to make it possible to convert between general events
     * and the various subclasses.  By keeping all event classes the same size,
     * this design avoids any issues of slicing off parts of the data during
     * such conversions.
     */

    /* General events */
    EventClassType eventClass;
    int eventType;
    int modifiers;
    double eventTime;
    bool valid;
    std::string sourceKey;

    /* Window, mouse, and key events */
    GWindowData* gwd;

    /* Action events */
    GObject* source;
    std::string actionCommand;

    /* Mouse events */
    double x;
    double y;

    /* Key events */
    int keyChar;
    int keyCode;

    /* Server events */
    int requestID;
    std::string requestUrl;

    /* Table events */
    int row;
    int column;
    std::string value;

    /* Timer events */
    GTimerData* gtd;

    /* Friend specifications */
    friend class GActionEvent;
    friend class GChangeEvent;
    friend class GHyperlinkEvent;
    friend class GKeyEvent;
    friend class GMouseEvent;
    friend class GScrollEvent;
    friend class GServerEvent;
    friend class GTableEvent;
    friend class GTimerEvent;
    friend class GWindowEvent;
};

/*
 * Function: getNextEvent
 * Usage: GEvent e = getNextEvent(mask);
 * -------------------------------------
 * Checks to see if there are any events of the desired type waiting on the
 * event queue.  If so, this function returns the event in exactly the same
 * fashion as <code>waitForEvent</code>; if not, <code>getNextEvent</code>
 * returns an invalid event.  The <code>mask</code> parameter is optional.
 * If it is missing, <code>getNextEvent</code> accepts any event.
 */
GEvent getNextEvent(int mask = ANY_EVENT);

/*
 * Function: waitForClick
 * Usage: waitForClick();
 * ----------------------
 * Waits for a mouse click in any window, discarding any other events.
 * Returns the mouse event that occurred.
 */
GMouseEvent waitForClick();

/*
 * Function: waitForEvent
 * Usage: GEvent e = waitForEvent(mask);
 * -------------------------------------
 * Dismisses the process until an event occurs whose type is covered by
 * the event mask.  The mask parameter is a combination of the events of
 * interest.  For example, to wait for a mouse event or an action event,
 * clients can use the following call:
 *
 *<pre>
 *    e = waitForEvent(MOUSE_EVENT + ACTION_EVENT);
 *</pre>
 *
 * The <code>mask</code> parameter is optional.  If it is missing,
 * <code>waitForEvent</code> accepts any event.
 *
 * <p>As a more sophisticated example, the following code is the canonical
 * event loop for an animated application that needs to respond to mouse,
 * key, and timer events:
 *
 *<pre>
 *    GTimer timer(ANIMATION_DELAY_IN_MILLISECONDS);
 *    timer.start();
 *    while (true) {
 *       GEvent e = waitForEvent(TIMER_EVENT + MOUSE_EVENT + KEY_EVENT);
 *       switch (e.getEventClass()) {
 *        case TIMER_EVENT:
 *          takeAnimationStep();
 *          break;
 *        case MOUSE_EVENT:
 *          handleMouseEvent(GMouseEvent(e));
 *          break;
 *        case KEY_EVENT:
 *          handleKeyEvent(GKeyEvent(e));
 *          break;
 *       }
 *    }
 *</pre>
 */
GEvent waitForEvent(int mask = ANY_EVENT);

/*
 * Class: GActionEvent
 * -------------------
 * This event subclass represents an action event.
 * Action events are generated by the classes in the
 * <a href="GInteractor-class.html"><code>GInteractor</code></a>
 * hierarchy.  As an example, the following program displays
 * a button that, when pushed, generates the message
 * &ldquo;Please do not press this button again&rdquo;
 * (with thanks to Douglas Adams&rsquo;s <i>Hitchhiker&rsquo;s
 * Guide to the Galaxy</i>):
 *
 *<pre>
 *    int main() {
 *       GWindow gw;
 *       GButton* button = new GButton("RED");
 *       gw.addToRegion(button, "SOUTH");
 *       while (true) {
 *          GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
 *          if (e.getEventType() == MOUSE_CLICKED) break;
 *          cout << "Please do not press this button again." << endl;
 *       }
 *       return 0;
 *    }
 *</pre>
 */
class GActionEvent : public GEvent {
public:
    /*
     * Constructor: GActionEvent
     * Usage: GActionEvent actionEvent(type, source, actionCommand);
     * -------------------------------------------------------------
     * Creates a <code>GActionEvent</code> using the specified parameters.
     */
    GActionEvent(EventType type, GObject* source, std::string actionCommand);

    /*
     * Method: getSource
     * Usage: GObject* gobj = e.getSource();
     * -------------------------------------
     * Returns a pointer to the <code>GObject</code> that generated this event.
     */
    GObject* getSource() const;

    /*
     * Method: getActionCommand
     * Usage: string cmd = e.getActionCommand();
     * -----------------------------------------
     * Returns the action command associated with this event.
     */
    std::string getActionCommand() const;

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    std::string toString() const;

    /* Private section */
    GActionEvent();
    GActionEvent(GEvent e);
};

/*
 * Class: GChangeEvent
 * -------------------
 * Change events that occur on an interactor with a state such as editable text.
 */
class GChangeEvent : public GEvent {
public:
    /*
     * Constructor: GChangeEvent
     * Usage: GChangeEvent changeEvent(type, source);
     * ----------------------------------------------
     * Creates a <code>GChangeEvent</code> with the specified type and source.
     */
    GChangeEvent(EventType type, GObject* source);

    /*
     * Method: getSource
     * Usage: GObject* gobj = e.getSource();
     * -------------------------------------
     * Returns a pointer to the <code>GObject</code> that generated this event.
     */
    GObject* getSource() const;

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    std::string toString() const;

    /* Private section */
    GChangeEvent();
    GChangeEvent(GEvent e);
};

/*
 * Class: GHyperlinkEvent
 * ----------------------
 * Change events that occur when a user clicks a hyperlink on an interactor
 * that allows linking, such as a GFormattedPane.
 */
class GHyperlinkEvent : public GEvent {
public:
    /*
     * Constructor: GHyperlinkEvent
     * Usage: GHyperlinkEvent hyperlinkEvent(type, source, url);
     * ---------------------------------------------------------
     * Creates a <code>GHyperlinkEvent</code> with the specified type,
     * source interactor, and link URL.
     */
    GHyperlinkEvent(EventType type, GObject* source, const std::string& url);

    /*
     * Method: getSource
     * Usage: GObject* gobj = e.getSource();
     * -------------------------------------
     * Returns a pointer to the <code>GObject</code> that generated this event.
     */
    GObject* getSource() const;

    /*
     * Method: getUrl
     * Usage: string url = e.getUrl();
     * -------------------------------
     * Returns the URL of the link that was clicked.
     */
    std::string getUrl() const;

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    std::string toString() const;

    /* Private section */
    GHyperlinkEvent();
    GHyperlinkEvent(GEvent e);
};

/*
 * Class: GKeyEvent
 * ----------------
 * This event subclass represents a key event.  Each key event records
 * the event type along with two representations of the key.  The
 * <code>getKeyChar</code> function is more generally useful and
 * returns the character after taking into account modifier keys.
 * The <code>getKeyCode</code> function returns an integer identifying
 * the key, which can be a function key as well as a standard key.
 * The codes return by <code>getKeyCode</code> are listed in the
 * <code>KeyCodes</code> enumeration.
 */
class GKeyEvent : public GEvent {
public:
    /*
     * Constructor: GKeyEvent
     * Usage: GKeyEvent keyEvent(type, gw, keyChar, keyCode);
     * ------------------------------------------------------
     * Creates a <code>GKeyEvent</code> using the specified parameters.
     */
    GKeyEvent(EventType type, const GWindow& gw, int keyChar, int keyCode);

    /*
     * Method: getGWindow
     * Usage: GWindow gw = e.getGWindow();
     * -----------------------------------
     * Returns the graphics window in which this event occurred.
     */
    GWindow getGWindow() const;

    /*
     * Method: getKeyChar
     * Usage: char ch = e.getKeyChar();
     * --------------------------------
     * Returns the character represented by the keystroke, taking the modifier
     * keys into account.  For example, if the user types the <code>'a'</code>
     * key with the shift key down, <code>getKeyChar</code> will return
     * <code>'A'</code>.  If the key code in the event does not correspond
     * to a character, <code>getKeyChar</code> returns the null character.
     */
    char getKeyChar() const;

    /*
     * Method: getKeyCode
     * Usage: int key = getKeyCode();
     * ------------------------------
     * Returns the integer code associated with the key in the event.
     */
    int getKeyCode() const;

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    std::string toString() const;

    /* Private section */
    GKeyEvent();
    GKeyEvent(GEvent e);
};

/*
 * Class: GMouseEvent
 * ------------------
 * This event subclass represents a mouse event.  Each mouse event
 * records the event type (<code>MOUSE_PRESSED</code>,
 * <code>MOUSE_RELEASED</code>, <code>MOUSE_CLICKED</code>,
 * <code>MOUSE_MOVED</code>, <code>MOUSE_DRAGGED</code>) along
 * with the coordinates of the event.  Clicking the mouse generates
 * three events in the following order: <code>MOUSE_PRESSED</code>,
 * <code>MOUSE_RELEASED</code>, <code>MOUSE_CLICKED</code>.
 *
 * <p>As an example, the following program uses mouse events to let
 * the user draw rectangles on the graphics window.  The only
 * complexity in this code is the use of the library functions
 * <code>min</code> and <code>abs</code> to ensure that the
 * dimensions of the rectangle are positive.
 *
 *<pre>
 *    int main() {
 *       GWindow gw;
 *       cout << "This program lets the user draw rectangles." << endl;
 *       GRect *rect;
 *       double startX;
 *       double startY;
 *       while (true) {
 *          GMouseEvent e = waitForEvent();
 *          if (e.getEventType() == MOUSE_PRESSED) {
 *             startX = e.getX();
 *             startY = e.getY();
 *             rect = new GRect(startX, startY, 0, 0);
 *             rect->setFilled(true);
 *             gw.add(rect);
 *          } else if (e.getEventType() == MOUSE_DRAGGED) {
 *             double x = min(e.getX(), startX);
 *             double y = min(e.getY(), startY);
 *             double width = abs(e.getX() - startX);
 *             double height = abs(e.getY() - startY);
 *             rect->setBounds(x, y, width, height);
 *          }
 *       }
 *    }
 *</pre>
 */
class GMouseEvent : public GEvent {
public:
    /*
     * Constructor: GMouseEvent
     * Usage: GMouseEvent mouseEvent(type, gw, x, y);
     * ----------------------------------------------
     * Creates a <code>GMouseEvent</code> using the specified parameters.
     */
    GMouseEvent(EventType type, const GWindow & gw, double x, double y);

    /*
     * Method: getGWindow
     * Usage: GWindow gw = e.getGWindow();
     * -----------------------------------
     * Returns the graphics window in which this event occurred.
     */
    GWindow getGWindow() const;

    /*
     * Method: getX
     * Usage: double x = getX();
     * -------------------------
     * Returns the <i>x</i> coordinate at which the event occurred relative
     * to the window origin at the upper left corner of the window.
     */
    double getX() const;

    /*
     * Method: getY
     * Usage: double y = getY();
     * -------------------------
     * Returns the <i>y</i> coordinate at which the event occurred relative
     * to the window origin at the upper left corner of the window.
     */
    double getY() const;

    /*
     * Returns true if the user pressed the left mouse button.
     */
    bool isLeftClick() const;

    /*
     * Returns true if the user pressed the middle mouse button.
     */
    bool isMiddleClick() const;

    /*
     * Returns true if the user pressed the right mouse button.
     */
    bool isRightClick() const;

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    std::string toString() const;

    /* Private section */
    GMouseEvent();
    GMouseEvent(GEvent e);
};

/*
 * Class: GScrollEvent
 * -------------------
 * Scroll events occur when the user scrolls a GScrollBar.
 */
class GScrollEvent : public GEvent {
public:
    /*
     * Constructor: GScrollEvent
     * Usage: GScrollEvent changeEvent(type, source);
     * ----------------------------------------------
     * Creates a <code>GScrollEvent</code> with the specified type and source.
     */
    GScrollEvent(EventType type, GObject* source);

    /*
     * Method: getSource
     * Usage: GObject* gobj = e.getSource();
     * -------------------------------------
     * Returns a pointer to the <code>GObject</code> that generated this event.
     */
    GObject* getSource() const;

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    std::string toString() const;

    /* Private section */
    GScrollEvent();
    GScrollEvent(GEvent e);
};

/*
 * Class: GServerEvent
 * -------------------
 * Events that occur on a web server.
 */
class GServerEvent : public GEvent {
public:
    /*
     * Constructor: GServerEvent
     * Usage: GServerEvent serverEvent(type, requestID, requestUrl);
     * -------------------------------------------------------------
     * Creates a <code>GServerEvent</code> with the specified type,
     * request ID, and request URL.
     */
    GServerEvent(EventType type, int requestID, const std::string& requestUrl);

    /*
     * Returns the request ID sent by the server to disambiguate messages.
     */
    int getRequestID() const;

    /*
     * Returns the URL of the request that the web client wants to fetch,
     * such as "/foo/bar/baz.txt".
     */
    std::string getRequestURL() const;

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    std::string toString() const;

    /* Private section */
    GServerEvent();
    GServerEvent(GEvent e);
};

/*
 * Class: GTableEvent
 * ------------------
 * Events that occur on a GTable.
 */
class GTableEvent : public GEvent {
public:
    /*
     * Constructor: GTableEvent
     * Usage: GTableEvent tableEvent(type);
     * -------------------------------------------
     * Creates a <code>GTableEvent</code> for the specified table.
     */
    GTableEvent(EventType type);

//    /*
//     * Method: getGTable
//     * Usage: GTable table = e.getGTable();
//     * ------------------------------------
//     * Returns the table that generated this event.
//     */
//    GTable getGTable() const;

    int getColumn() const;

    int getRow() const;

    std::string getValue() const;

    void setLocation(int row, int column);

    void setValue(std::string value);

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    std::string toString() const;

    /* Private section */
    GTableEvent();
    GTableEvent(GEvent e);
};

/*
 * Class: GTimerEvent
 * ------------------
 * This event subclass represents a timer event.  Timer events are
 * generated by a <a href="GTimer-class.html"><code>GTimer</code></a>
 * object, which produces a new event at a fixed interval measured in
 * milliseconds.  As an example, the following program generates a
 * timer event every two seconds, stopping when the user clicks
 * somewhere in the window:
 *
 *<pre>
 *    int main() {
 *       cout << "This program generates timer events." << endl;
 *       GTimer timer(2000);
 *       timer.start();
 *       while (true) {
 *          GEvent e = waitForEvent(CLICK_EVENT | TIMER_EVENT);
 *          if (e.getEventType() == MOUSE_CLICKED) break;
 *          cout << "Timer ticked" << endl;
 *       }
 *       return 0;
 *    }
 *</pre>
 */
class GTimerEvent : public GEvent {
public:
    /*
     * Constructor: GTimerEvent
     * Usage: GTimerEvent timerEvent(type, timer);
     * -------------------------------------------
     * Creates a <code>GTimerEvent</code> for the specified timer.
     */
    GTimerEvent(EventType type, const GTimer& timer);

    /*
     * Method: getGTimer
     * Usage: GTimer timer = e.getGTimer();
     * ------------------------------------
     * Returns the timer that generated this event.
     */
    GTimer getGTimer() const;

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    std::string toString() const;

    /* Private section */
    GTimerEvent();
    GTimerEvent(GEvent e);
};

/*
 * Class: GWindowEvent
 * -------------------
 * This event subclass represents a window event.
 * Each <code>GWindowEvent</code> keeps track of the event type
 * (<code>WINDOW_CLOSED</code>, <code>WINDOW_RESIZED</code>) along
 * with the identity of the window.
 */
class GWindowEvent : public GEvent {
public:
    /*
     * Constructor: GWindowEvent
     * Usage: GWindowEvent windowEvent(type, gw);
     * ------------------------------------------
     * Creates a <code>GWindowEvent</code> using the specified parameters.
     */
    GWindowEvent(EventType type, const GWindow & gw);

    /*
     * Method: getGWindow
     * Usage: GWindow gw = e.getGWindow();
     * -----------------------------------
     * Returns the graphics window in which this event occurred.
     */
    GWindow getGWindow() const;

    /*
     * Method: toString
     * Usage: string str = e.toString();
     * ---------------------------------
     * Converts the event to a human-readable representation of the event.
     */
    std::string toString() const;

    /* Private section */
    GWindowEvent();
    GWindowEvent(GEvent e);
};

/*
 * Operator to output an event to the console or a stream.
 */
std::ostream& operator <<(std::ostream& out, const GEvent& event);
std::ostream& operator <<(std::ostream& out, const GActionEvent& event);
std::ostream& operator <<(std::ostream& out, const GKeyEvent& event);
std::ostream& operator <<(std::ostream& out, const GMouseEvent& event);
std::ostream& operator <<(std::ostream& out, const GServerEvent& event);
std::ostream& operator <<(std::ostream& out, const GTableEvent& event);
std::ostream& operator <<(std::ostream& out, const GTimerEvent& event);
std::ostream& operator <<(std::ostream& out, const GWindowEvent& event);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gevents_h
