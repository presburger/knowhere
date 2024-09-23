// Copyright (C) 2019-2023 Zilliz. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under the License.

#include "knowhere/index/handle.h"

#include "folly/futures/Future.h"
/*
class Handle {
 public:
    explicit Handle(const std::chrono::seconds& timeout);

    void
    Stop();

    bool
    Flag() const;

    bool
    IsTimeout() const;

    Status
    Get();

    void
    Set(std::unique_ptr<folly::Future<Status>>&& future);

    ~Handle();

 private:
    std::chrono::steady_clock::time_point start;
    std::chrono::seconds timeout;
    std::unique_ptr<folly::Future<Status>> future = nullptr;
    mutable std::atomic_bool flag = false;
};
 */

namespace knowhere {
Handle::Handle(const std::chrono::seconds& timeout)
    : start(std::chrono::steady_clock::now()), timeout(timeout), future(std::move(future)) {
}

void
Handle::Stop() {
    this->flag.store(true);
};

bool
Handle::Flag() const {
    return this->flag.load();
}

bool
Handle::IsTimeout() const {
    auto now = std::chrono::steady_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::seconds>(now - start);
    return dur.count() > timeout.count();
}

Status
Handle::Get() {
    return std::move(*future).get();
}

void
Handle::Set(folly::Future<Status>&& future) {
    this->future = std::make_unique<folly::Future<Status>>(std::move(future));
}

Handle::~Handle() {
}

}  // namespace knowhere
