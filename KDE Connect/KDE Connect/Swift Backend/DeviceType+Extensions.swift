/*
 * SPDX-FileCopyrightText: 2021 Lucas Wang <lucas.wang@tuta.io>
 * SPDX-FileCopyrightText: 2022 Claudio Cambra <claudio.cambra@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

//
//  DeviceType+Extensions.swift
//  KDE Connect
//
//  Created by Claudio Cambra on 25/5/22.
//

import Foundation

public extension DeviceType {
    var sfSymbolName: String {
        switch (self) {
            case .unknown: return "questionmark.square.dashed"
            case .desktop: return "desktopcomputer"
            case .laptop: return "laptopcomputer"
            case .phone: return "apps.iphone"
            case .tablet: return "apps.ipad.landscape"
            case .tv: return "tv"
            @unknown default: return "questionmark.square.dashed"
        }
    }

    static var current: DeviceType {
        var macDeviceType: DeviceType {
            "hw.model".withCString { hwModelCStr in
                var size = 0
                if sysctlbyname(hwModelCStr, nil, &size, nil, 0) != 0 {
                    print("Failed to get size of hw.model (\(String(cString: strerror(errno))))")
                    return .unknown
                }
                precondition(size > 0)
                var resultCStr = [CChar](repeating: 0, count: size)
                if sysctlbyname(hwModelCStr, &resultCStr, &size, nil, 0) != 0 {
                    print("Failed to get hw.model (\(String(cString: strerror(errno))))")
                    return .unknown
                }
                // https://everymac.com/systems/by_capability/mac-specs-by-machine-model-machine-id.html
                switch String(cString: resultCStr) {
                case let model where model.starts(with: "MacBook"):
                    return .laptop
                case let model where model.contains("Mac"):
                    return .desktop
                case let model:
                    print("Unexpected hw.model (\(model)")
                    return .unknown
                }
            }
        }
        switch UIDevice.current.userInterfaceIdiom {
        case .unspecified:
            return .unknown
        case .phone:
            return .phone
        case .pad:
            let processInfo = ProcessInfo.processInfo
            if processInfo.isMacCatalystApp || processInfo.isiOSAppOnMac {
                return macDeviceType
            }
            return .tablet
        case .tv:
            return .tv
        case .carPlay:
            return .unknown
        case .mac:
            return macDeviceType
        @unknown default:
            return .unknown
        }
    }
}