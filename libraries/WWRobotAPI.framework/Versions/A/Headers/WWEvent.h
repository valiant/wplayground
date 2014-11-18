//
//  WWEvent.h
//  APIObjectiveC
//
//  Created by Kevin Liang on 3/31/14.
//  Copyright (c) 2014 Wonder Workshop inc. (https://www.makewonder.com/) All rights reserved.
//

#import "WWObject.h"

@class WWEvent, WWSensorHistory;

typedef BOOL(^WWEventAlertBlock)(WWEvent *event, WWSensorHistory *history);


/**
 * Certain events recognize "phases" in the event, and can be configured to trigger upon entry into any of these phases.
 *
 * As of this writing, these events are `gestureSlideAlongAxis` and `gestureDrop`.
 */
typedef enum {
    WW_EVENT_GESTURE_IDLE         = 1 << 0,      /// nothing is happening; waiting for event to start.
    WW_EVENT_GESTURE_STARTED      = 1 << 1,      /// the gesture might be starting. but it might be some other gesture..
    WW_EVENT_GESTURE_ESTABLISHED  = 1 << 2,      /// the gesture has definitely started.
    WW_EVENT_GESTURE_COMPLETED    = 1 << 3,      /// the gesture has finished.
    WW_EVENT_GESTURE_CANCELLED    = 1 << 4,      /// the gesture started, but it was a false positive - it wasn't this gesture after all.
} WWEventPhase;

typedef unsigned int WWEventPhaseMask; /// bitwise-OR of `WWEventPhase` enums.

/**
 *  The `WWEvent` object registers `WWRobot` for event notification.
 *
 *  Because a registered `WWEvent` is evaluated on every arrival of `WWSensor` data for `WWRobot`,
 *  it is highly recommended to make sure the shouldAlertBlock execution code is relatively lightweight
 *  and non-blocking to prevent performance degradation.
 *
 */
@interface WWEvent : WWObject

/**
 *  Specifies the unique identifier for this given `WWEvent` (optional).
 */
@property (nonatomic, strong) NSString *identifier;

/**
 *  Carries any additional information to use during alert block execution (optional).
 */
@property (nonatomic, strong) NSMutableDictionary *information;

/**
 *  The main execution block of `WWEvent`.  If true, robot:eventsTriggered: method of `WWRobotDelegate` is invoked.
 */
@property (copy) WWEventAlertBlock shouldAlertBlock;

/**
 *  Returns the timestamp when this event became active (read-only).
 */
@property (nonatomic, readonly) NSDate *timestamp;

/**
 *  Returns whether this event is active (read-only). 
 */
@property (nonatomic, readonly) BOOL isActive;

/**
 *  The current `WWEventPhase` of the event.
 */
@property (nonatomic, readonly) WWEventPhase eventPhase;

/**
 * Bitwise-OR of the `WWEventPhase`
 */
@property (nonatomic) WWEventPhaseMask signalForPhaseMask;    // which Phases we should emit an event for.


/**
 *  Initializes a new instance of `WWEvent` with the specified event block.
 *
 *  Identifier will be initialized to empty string.
 *
 *  @param block The execution block of the event.
 *
 *  @return Returns a newly initialized `WWEvent`.
 */
- (id) initWithShouldAlertBlock:(WWEventAlertBlock)block;

/**
 *  Initializes a new instance of `WWEvent` with a specified event block and identifier.
 *
 *  @param block The execution block of the event.
 *  @param identifier The string identifier for event creator.
 *
 *  @return Returns a newly initialized `WWEvent`.
 */
- (id) initWithShouldAlertBlock:(WWEventAlertBlock)block identifier:(NSString *)identifier;

/**
 * Tells the `WWEvent` to trigger when it enters specific `WWEventPhase`s
 * Certain events recognize "phases" in the event, and can be configured to trigger upon entry into any of these phases.
 *
 * As of this writing, these events are `gestureSlideAlongAxis` and `gestureDrop`.
 * - WW_EVENT_GESTURE_IDLE
 *   nothing is happening; waiting for event to start.
 * - WW_EVENT_GESTURE_STARTED
 *   the gesture might be starting. but it might be some other gesture..
 * - WW_EVENT_GESTURE_ESTABLISHED
 *   the gesture has definitely started.
 * - WW_EVENT_GESTURE_COMPLETED
 *   the gesture has finished.
 * - WW_EVENT_GESTURE_CANCELLED
 *   the gesture started, but it was a false positive - it wasn't this gesture after all.
 *
 *  @param mask bitwise-OR of `WWEventPhase` enums.
 *
 *  @return returns the `WWEvent` itself.
 */
- (WWEvent*) setPhaseSignalMask:(WWEventPhaseMask)mask;

/**
 * Convert a `WWEventPhase` enum to an `NSString`.
 *
 * @param phase The `WWEventPhase` to convert.
 *
 * @return `NSString` human-readable phase name.
 */
+ (NSString *) eventPhaseToString:(WWEventPhase)phase;

/**
 *  Returns true if the given identifier is the same as the event's identifier.
 * 
 *  String comparison is case-insensitive.
 *
 *  @param identifier The string to be compared against.
 *
 *  @return Returns true if given identifier matches event's identifier.
 */
- (BOOL) hasIdentifier:(NSString *)identifier;

@end
