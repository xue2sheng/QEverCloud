/**
 * Original work: Copyright (c) 2014 Sergey Skoblikov
 * Modified work: Copyright (c) 2015-2019 Dmitry Ivanov
 *
 * This file is a part of QEverCloud project and is distributed under the terms
 * of MIT license:
 * https://opensource.org/licenses/MIT
 */

#include <EverCloudException.h>
#include <memory>

namespace qevercloud {

EverCloudException::EverCloudException() :
    m_error()
{}

EverCloudException::EverCloudException(QString error) :
    m_error(error.toUtf8())
{}

EverCloudException::EverCloudException(const std::string & error) :
    m_error(error.c_str())
{}

EverCloudException::EverCloudException(const char * error) :
    m_error(error)
{}

EverCloudException::~EverCloudException() throw()
{}

const char * EverCloudException::what() const throw()
{
    return m_error.constData();
}

EverCloudExceptionDataPtr EverCloudException::exceptionData() const
{
    return std::make_shared<EverCloudExceptionData>(
        QString::fromUtf8(what()));
}

EverCloudExceptionData::EverCloudExceptionData(QString error) :
    errorMessage(error)
{}

void EverCloudExceptionData::throwException() const
{
    throw EverCloudException(errorMessage);
}

EvernoteException::EvernoteException() :
    EverCloudException()
{}

EvernoteException::EvernoteException(QString error) :
    EverCloudException(error)
{}

EvernoteException::EvernoteException(const std::string & error) :
    EverCloudException(error)
{}

EvernoteException::EvernoteException(const char * error) :
    EverCloudException(error)
{}

EverCloudExceptionDataPtr EvernoteException::exceptionData() const
{
    return std::make_shared<EverCloudExceptionData>(
        QString::fromUtf8(what()));
}

EvernoteExceptionData::EvernoteExceptionData(QString error) :
    EverCloudExceptionData(error)
{}

void EvernoteExceptionData::throwException() const
{
    throw EvernoteException(errorMessage);
}

} // namespace qevercloud
