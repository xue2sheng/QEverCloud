/**
 * Copyright (c) 2019 Dmitry Ivanov
 *
 * This file is a part of QEverCloud project and is distributed under the terms
 * of MIT license: https://opensource.org/licenses/MIT
 */

#ifndef QEVERCLOUD_DURABLE_SERVICE_H
#define QEVERCLOUD_DURABLE_SERVICE_H

#include "AsyncResult.h"
#include "Export.h"
#include "RequestContext.h"

#include <QDateTime>
#include <QSharedPointer>
#include <QVariant>

#include <functional>
#include <utility>

namespace qevercloud {

////////////////////////////////////////////////////////////////////////////////

struct QEVERCLOUD_EXPORT IRetryPolicy
{
    virtual bool shouldRetry(
        QSharedPointer<EverCloudExceptionData> exceptionData) = 0;
};

using IRetryPolicyPtr = QSharedPointer<IRetryPolicy>;

////////////////////////////////////////////////////////////////////////////////

QT_FORWARD_DECLARE_CLASS(DurableServicePrivate)

class QEVERCLOUD_EXPORT IDurableService
{
public:
    using SyncResult = std::pair<QVariant, QSharedPointer<EverCloudExceptionData>>;
    using SyncServiceCall = std::function<SyncResult(IRequestContextPtr)>;
    using AsyncServiceCall = std::function<AsyncResult*(IRequestContextPtr)>;

    struct QEVERCLOUD_EXPORT SyncRequest
    {
        const char *    m_name;
        QString         m_description;
        SyncServiceCall m_call;

        SyncRequest(const char * name, QString description,
                    SyncServiceCall && call) :
            m_name(name),
            m_description(std::move(description)),
            m_call(std::move(call))
        {}
    };

    struct QEVERCLOUD_EXPORT AsyncRequest
    {
        const char *    m_name;
        QString         m_description;
        AsyncServiceCall    m_call;

        AsyncRequest(const char * name, QString description,
                     AsyncServiceCall && call) :
            m_name(name),
            m_description(std::move(description)),
            m_call(std::move(call))
        {}
    };

public:
    virtual SyncResult executeSyncRequest(
        SyncRequest && syncRequest, IRequestContextPtr ctx) = 0;

    virtual AsyncResult * executeAsyncRequest(
        AsyncRequest && asyncRequest, IRequestContextPtr ctx) = 0;
};

using IDurableServicePtr = QSharedPointer<IDurableService>;

////////////////////////////////////////////////////////////////////////////////

QEVERCLOUD_EXPORT IRetryPolicyPtr newRetryPolicy();

QEVERCLOUD_EXPORT IDurableServicePtr newDurableService(
    IRetryPolicyPtr = {},
    IRequestContextPtr = {});

} // namespace qevercloud

#endif // QEVERCLOUD_DURABLE_SERVICE_H
