/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "devattest_system_ability_listener.h"

#include <thread>
#include <cstdint>
#include "net_conn_client.h"
#include "system_ability_definition.h"
#include "iservice_registry.h"
#include "devattest_log.h"
#include "devattest_errno.h"
#include "devattest_network_callback.h"

namespace OHOS {
namespace DevAttest {
using namespace NetManagerStandard;
constexpr std::int32_t WAIT_FOR_KVSTORE = 1000;
constexpr std::int32_t RETRY_REGISTER_NET_CALLBACK_TIME = 5;

void DevAttestSystemAbilityListener::OnAddSystemAbility(int32_t systemAbilityId, const std::string& deviceId)
{
    if (systemAbilityId == COMM_NET_CONN_MANAGER_SYS_ABILITY_ID) {
        std::shared_ptr<NetManagerStandard::NetConnClient> netManager = DelayedSingleton<NetConnClient>::GetInstance();
        if (netManager == nullptr) {
            HILOGE("Failed to init NetConnClient.");
            return;
        }

        sptr<DevAttestNetworkCallback> callback = (std::make_unique<DevAttestNetworkCallback>()).release();
        int32_t ret = 0;
        for (size_t i = 0; i < RETRY_REGISTER_NET_CALLBACK_TIME; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_FOR_KVSTORE));
            ret = netManager->RegisterNetConnCallback(callback);
            if (ret == NETMANAGER_SUCCESS) {
                break;
            }
        }

        if (ret != NETMANAGER_SUCCESS) {
            HILOGE("RegisterNetConnCallback failed.");
        }
    } else {
        HILOGW("Do Nothing");
    }

    if (RemoveDevAttestSystemAbilityListener(systemAbilityId)) {
        HILOGI("RemoveSystemAbilityListener success.");
    }
    return;
}

void DevAttestSystemAbilityListener::OnRemoveSystemAbility(int32_t systemAbilityId, const std::string& deviceId)
{
    HILOGI("SA:%{public}d removed", systemAbilityId);
}

bool DevAttestSystemAbilityListener::AddDevAttestSystemAbilityListener(int32_t systemAbilityId)
{
    HILOGD("AddDevAttestSystemAbilityListener start");
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGE("systemAbilityId invalid %{public}d", systemAbilityId);
        return false;
    }
    sptr<ISystemAbilityManager> samgrProxy = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (samgrProxy == nullptr) {
        HILOGE("failed to get samgrProxy");
        return false;
    }
    int32_t ret = samgrProxy->SubscribeSystemAbility(systemAbilityId, this);
    if (ret != DEVATTEST_SUCCESS) {
        HILOGE("failed to subscribe sa: %{public}d", systemAbilityId);
        return false;
    }
    return true;
}

bool DevAttestSystemAbilityListener::RemoveDevAttestSystemAbilityListener(int32_t systemAbilityId)
{
    if (!CheckInputSysAbilityId(systemAbilityId)) {
        HILOGE("systemAbilityId invalid %{public}d", systemAbilityId);
        return false;
    }
    sptr<ISystemAbilityManager> samgrProxy = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (samgrProxy == nullptr) {
        HILOGE("failed to get samgrProxy");
        return false;
    }
    int32_t ret = samgrProxy->UnSubscribeSystemAbility(systemAbilityId, this);
    if (ret != DEVATTEST_SUCCESS) {
        HILOGE("failed to unsubscribe sa: %{public}d", systemAbilityId);
        return false;
    }
    return true;
}

bool DevAttestSystemAbilityListener::CheckInputSysAbilityId(int32_t systemAbilityId)
{
    return (systemAbilityId >= FIRST_SYS_ABILITY_ID) && (systemAbilityId <= LAST_SYS_ABILITY_ID);
}
} // DevAttest
} // OHOS