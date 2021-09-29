//Copyright 29/4/14  YANG Qiao yangqiao0505@me.com
//kdeconnect is distributed under two licenses.
//
//* The Mozilla Public License (MPL) v2.0
//
//or
//
//* The General Public License (GPL) v2.1
//
//----------------------------------------------------------------------
//
//Software distributed under these licenses is distributed on an "AS
//IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
//implied. See the License for the specific language governing rights
//and limitations under the License.
//kdeconnect is distributed under both the GPL and the MPL. The MPL
//notice, reproduced below, covers the use of either of the licenses.
//
//---------------------------------------------------------------------

#import <Foundation/Foundation.h>
#import "BaseLink.h"
#import "NetworkPackage.h"
//#import "deviceDelegate.h"
//#import "KDE_Connect-Swift.h"
//#import "BackgroundService.h"
@class BaseLink;
@class NetworkPackage;
//@protocol Plugin;
//@class Ping;
//@class Share;
//@class FindMyPhone;
//@class Battery;
//@interface PluginInterface;

typedef NS_ENUM(NSUInteger, PairStatus)
{
    NotPaired=0,
    Requested=1,
    RequestedByPeer=2,
    Paired=3
};

typedef NS_ENUM(NSUInteger, DeviceType)
{
    Unknown=0,
    Desktop=1,
    Laptop=2,
    Phone=3,
    Tablet=4
};

typedef NS_ENUM(NSUInteger, HapticStyle)
{
    light = 0,
    medium = 1,
    heavy = 2,
    soft = 3,
    rigid = 4
};

@class ConnectedDevicesViewModel;

@interface Device : NSObject <linkDelegate, NSSecureCoding>

@property(readonly,nonatomic) NSString* _id;
@property(readonly,nonatomic) NSString* _name;
@property(readonly,nonatomic) DeviceType _type;
@property(readonly,nonatomic) NSInteger _protocolVersion;
@property(readonly,nonatomic) PairStatus _pairStatus;
@property(readonly,nonatomic) NSArray* _incomingCapabilities;
@property(readonly,nonatomic) NSArray* _outgoingCapabilities;

@property(nonatomic) NSMutableArray* _links;
@property(nonatomic) NSMutableDictionary* _plugins;
@property(nonatomic) NSMutableArray* _failedPlugins;

@property(nonatomic, copy) NSString* _SHA256HashFormatted;

// This is a generic pointer cause apperently neither the previous dev or I could figure out how to mutually import BackgroundService and Device with each other....
@property(nonatomic) id _deviceDelegate;
@property(nonatomic,assign) ConnectedDevicesViewModel* _backgroundServiceDelegate;

//@property(readonly,nonatomic) BOOL _testDevice;

// Plugin enable status
@property(nonatomic) NSMutableDictionary* _pluginsEnableStatus;

// Plugin-specific persistent data are stored in the Device object. Plugin objects contain runtime
// data only and are therefore NOT stored persistently
// Remote Input
@property(nonatomic) float _cursorSensitivity;
@property(nonatomic) HapticStyle _hapticStyle;
// Presenter
@property(nonatomic) float _pointerSensitivity;

// For NSCoding
@property (class, readonly) BOOL supportsSecureCoding;

//- (Device*) initTest;
- (Device*) init:(NSString*)deviceId setDelegate:(id)deviceDelegate;
- (Device*) init:(NetworkPackage*)np baselink:(BaseLink*)link setDelegate:(id)deviceDelegate;
- (NSInteger) compareProtocolVersion;

#pragma mark Link-related Functions
- (void) addLink:(NetworkPackage*)np baseLink:(BaseLink*)link;
- (void) onPackageReceived:(NetworkPackage*)np;
- (void) onLinkDestroyed:(BaseLink *)link;
- (void) onSendSuccess:(long)tag;
- (BOOL) sendPackage:(NetworkPackage*)np tag:(long)tag;
- (BOOL) isReachable;

#pragma mark Pairing-related Functions
- (BOOL) isPaired;
- (BOOL) isPaireRequested;
//- (void) setAsPaired; // Is this needed to be public?
- (void) requestPairing;
- (void) justChangeStatusToUnpaired;
- (void) unpair;
- (void) acceptPairing;
- (void) rejectPairing;

#pragma mark Plugin-related Functions
- (void) reloadPlugins;
// - (NSArray*) getPluginViews:(UIViewController*)vc;

#pragma mark enum tools
+ (NSString*)Devicetype2Str:(DeviceType)type;
+ (DeviceType)Str2Devicetype:(NSString*)str;
@end

@protocol deviceDelegate <NSObject>
@optional
- (void) onDeviceReachableStatusChanged:(Device*)device;
- (void) onDevicePairRequest:(Device*)device;
- (void) onDevicePairTimeout:(Device*)device;
- (void) onDevicePairSuccess:(Device*)device;
- (void) onDevicePairRejected:(Device*)device;
- (void) onDevicePluginChanged:(Device*)device;
@end