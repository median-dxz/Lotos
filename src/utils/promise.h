#ifndef PROMISE_H
#define PROMISE_H

#include "base.h"

#include <QFuture>
#include <QFutureWatcher>
#include <QObject>
#include <QtConcurrent>

#include <exception>
#include <functional>
#include <type_traits>

template <typename ResolverType, typename RejectorType = void>
class Promise;

class PromiseHelper : public QObject {
   public:
    PromiseHelper(QObject *obj) : QObject(obj), obj(obj) { watcher = new QFutureWatcher<void>(obj); }
    QObject *obj;
    QFutureWatcher<void> *watcher;
    QFuture<void> future;

    enum STATE { Unset, Pending, Fuifulled, Rejeced };
    STATE state = Unset;

    template <typename P>
    void setPromise(P *self) {
        connect(watcher, &QFutureWatcher<void>::finished, obj, [self, this] {
            if (state == Fuifulled || state == Pending) {
                state = Fuifulled;
                if (self->finishedCallback)
                    self->finishedCallback();
            }
            if (state == Rejeced) {
                if (self->failedCallback)
                    self->failedCallback();
            }
        });
        connect(watcher, &QFutureWatcher<void>::started, obj, [self] {
            if (self->startedCallback)
                self->startedCallback();
        });
        connect(watcher, &QFutureWatcher<void>::finished, this, &QObject::deleteLater);
        connect(this, &QObject::destroyed, this, [=] { delete self; });
    };

    template <class V>
    void transValue(V &&value, Promise<V> *self) {
        self->resolvedValue = std::move(value);
    }

    template <class V>
    void transReason(V &&value, Promise<V> *self) {
        self->rejectedReason = std::move(value);
    }

   public slots:
    void start() {
        if (state == Unset)
            state = Pending;
        watcher->setFuture(future);
    };
};

template <typename ResolverType, typename RejectorType>
class Promise {
   public:
    Promise(QObject *obj) { d = new PromiseHelper(obj); }
    virtual ~Promise() {}

    using Resolver = std::function<void(ResolverType)>;
    using Rejector = std::function<void()>;
    using async = std::function<void(Resolver, Rejector)>;
    using callback = std::function<void()>;

    using STATE = PromiseHelper::STATE;
    using result = ResolverType &;
    using reason = RejectorType &;

    void setPromise(async executor) {
        if (d->state == STATE::Unset) {
            resolver = std::bind(
                [=](ResolverType &&value, PromiseHelper *d) {
                    if (d->state == PromiseHelper::Pending) {
                        d->state = PromiseHelper::Fuifulled;
                        d->transValue(std::forward<ResolverType>(value), this);
                    }
                },
                std::placeholders::_1, d);
            rejector = std::bind(
                [=](RejectorType &&reason, PromiseHelper *d) {
                    if (d->state == PromiseHelper::Pending) {
                        d->state = PromiseHelper::Rejeced;
                        d->transReason(std::forward<RejectorType>(reason), this);
                    }
                },
                std::placeholders::_1, d);

            d->setPromise(this);
            d->future = QtConcurrent::run(executor, resolver, rejector);
            d->start();
        }
    }

    void onStarted(std::function<void()> func) {
        if (d->state == STATE::Unset) {
            startedCallback = func;
        }
    }

    void onFinished(std::function<void(result)> func) {
        if (d->state == STATE::Unset) {
            finishedCallback = [this, func] { func(this->resolvedValue); };
        }
    }

    void onFailed(std::function<void()> func) {
        if (d->state == STATE::Unset) {
            failedCallback = [this, func] { func(this->rejectedReason); };
        }
    }

   private:
    PromiseHelper *d;
    Resolver resolver;
    Rejector rejector;
    callback startedCallback = nullptr;
    callback finishedCallback = nullptr;
    callback failedCallback = nullptr;
    ResolverType resolvedValue;
    RejectorType rejectedReason;
    friend class PromiseHelper;
};

template <typename ResolverType>
class Promise<ResolverType, void> {
   public:
    Promise(QObject *obj) { d = new PromiseHelper(obj); }
    virtual ~Promise() {}

    using Resolver = std::function<void(ResolverType)>;
    using Rejector = std::function<void()>;
    using async = std::function<void(Resolver, Rejector)>;
    using callback = std::function<void()>;

    using STATE = PromiseHelper::STATE;
    using result = ResolverType &;

    void setPromise(async executor) {
        if (d->state == STATE::Unset) {
            resolver = std::bind(
                [=](ResolverType &&value, PromiseHelper *d) {
                    if (d->state == PromiseHelper::Pending) {
                        d->state = PromiseHelper::Fuifulled;
                        d->transValue(std::forward<ResolverType>(value), this);
                    }
                },
                std::placeholders::_1, d);
            rejector = [=]() {
                if (d->state == PromiseHelper::Pending) {
                    d->state = PromiseHelper::Rejeced;
                }
            };

            d->setPromise(this);
            d->future = QtConcurrent::run(executor, resolver, rejector);
            d->start();
        }
    }

    void onStarted(std::function<void()> func) {
        if (d->state == STATE::Unset) {
            startedCallback = func;
        }
    }

    void onFinished(std::function<void(result)> func) {
        if (d->state == STATE::Unset) {
            finishedCallback = [this, func] { func(this->resolvedValue); };
        }
    }

    void onFailed(std::function<void()> func) {
        if (d->state == STATE::Unset) {
            failedCallback = [this, func] { func(); };
        }
    }

   private:
    PromiseHelper *d;
    Resolver resolver;
    Rejector rejector;
    callback startedCallback = nullptr;
    callback finishedCallback = nullptr;
    callback failedCallback = nullptr;
    ResolverType resolvedValue;
    friend class PromiseHelper;
};

template <>
class Promise<void, void> {
   public:
    Promise(QObject *obj) { d = new PromiseHelper(obj); }
    virtual ~Promise() {}

    using Resolver = std::function<void()>;
    using Rejector = std::function<void()>;
    using async = std::function<void(Resolver, Rejector)>;
    using callback = std::function<void()>;

    using STATE = PromiseHelper::STATE;

    void setPromise(async executor) {
        if (d->state == STATE::Unset) {
            resolver = [=]() {
                if (d->state == PromiseHelper::Pending) {
                    d->state = PromiseHelper::Fuifulled;
                }
            };
            rejector = [=]() {
                if (d->state == PromiseHelper::Pending) {
                    d->state = PromiseHelper::Rejeced;
                }
            };

            d->setPromise(this);
            d->future = QtConcurrent::run(executor, resolver, rejector);
            d->start();
        }
    }

    void onStarted(std::function<void()> func) {
        if (d->state == STATE::Unset) {
            startedCallback = func;
        }
    }

    void onFinished(std::function<void()> func) {
        if (d->state == STATE::Unset) {
            finishedCallback = func;
        }
    }

    void onFailed(std::function<void()> func) {
        if (d->state == STATE::Unset) {
            failedCallback = func;
        }
    }

   private:
    PromiseHelper *d;
    Resolver resolver;
    Rejector rejector;
    callback startedCallback = nullptr;
    callback finishedCallback = nullptr;
    callback failedCallback = nullptr;
    friend class PromiseHelper;
};

#endif  // PROMISE_H
