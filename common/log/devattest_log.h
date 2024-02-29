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

#ifndef DEVATTEST_LOG_H
#define DEVATTEST_LOG_H

#include <string>
#include "hilog/log.h"

#undef LOG_TAG
#define LOG_TAG "DEVATTEST"

#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD005D00

namespace OHOS {
namespace DevAttest {
#define HILOGF(fmt, ...) HILOG_FATAL(LOG_CORE, fmt, ##__VA_ARGS__)
#define HILOGE(fmt, ...) HILOG_ERROR(LOG_CORE, fmt, ##__VA_ARGS__)
#define HILOGW(fmt, ...) HILOG_WARN(LOG_CORE, fmt, ##__VA_ARGS__)
#define HILOGI(fmt, ...) HILOG_INFO(LOG_CORE, fmt, ##__VA_ARGS__)
#define HILOGD(fmt, ...) HILOG_DEBUG(LOG_CORE, fmt, ##__VA_ARGS__)
} // end of DevAttest
} // end of OHOS
#endif