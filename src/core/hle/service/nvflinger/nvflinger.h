// Copyright 2018 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <memory>
#include <boost/optional.hpp>
#include "core/hle/kernel/event.h"

namespace CoreTiming {
struct EventType;
}

namespace Service {
namespace NVFlinger {

class BufferQueue;

struct Layer {
    Layer(u64 id, std::shared_ptr<BufferQueue> queue);
    ~Layer() = default;

    u64 id;
    std::shared_ptr<BufferQueue> buffer_queue;
};

struct Display {
    Display(u64 id, std::string name);
    ~Display() = default;

    u64 id;
    std::string name;

    std::vector<Layer> layers;
    Kernel::SharedPtr<Kernel::Event> vsync_event;
};

class NVFlinger final {
public:
    NVFlinger();
    ~NVFlinger();

    /// Opens the specified display and returns the id.
    u64 OpenDisplay(const std::string& name);

    /// Creates a layer on the specified display and returns the layer id.
    u64 CreateLayer(u64 display_id);

    /// Gets the buffer queue id of the specified layer in the specified display.
    u32 GetBufferQueueId(u64 display_id, u64 layer_id);

    /// Gets the vsync event for the specified display.
    Kernel::SharedPtr<Kernel::Event> GetVsyncEvent(u64 display_id);

    /// Obtains a buffer queue identified by the id.
    std::shared_ptr<BufferQueue> GetBufferQueue(u32 id) const;

    /// Performs a composition request to the emulated nvidia GPU and triggers the vsync events when
    /// finished.
    void Compose();

private:
    /// Returns the display identified by the specified id.
    Display& GetDisplay(u64 display_id);

    /// Returns the layer identified by the specified id in the desired display.
    Layer& GetLayer(u64 display_id, u64 layer_id);

    std::vector<Display> displays;
    std::vector<std::shared_ptr<BufferQueue>> buffer_queues;

    /// Id to use for the next layer that is created, this counter is shared among all displays.
    u64 next_layer_id = 1;
    /// Id to use for the next buffer queue that is created, this counter is shared among all
    /// layers.
    u32 next_buffer_queue_id = 1;

    /// CoreTiming event that handles screen composition.
    CoreTiming::EventType* composition_event;
};

} // namespace NVFlinger
} // namespace Service
