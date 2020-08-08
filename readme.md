# directx-wrapper

"directx-wrapper" is a lightweight library to wrap the DirectX Graphics API and make it easy to program with DirectX.

## Feature

Create the objects easily.

```C++
const auto device = directx12::device::create(feature_level);
...
const auto swap_chain = directx12::swap_chain::create(queue, width, height, HWND);
```

Create the class of wrapper with source objects.

```C++
const auto swap_chain = directx12::swap_chain(const ComPtr<IDXGISwapChain4>&...);
```

Use `->` to access the member of source objects in the class of wrapper.

```C++
command_list->DrawInstanced(...); //command_list is directx12::graphics_command_list.
```

Use `xxxInfo` to set the properties of objects(like `xxx_Desc`).

```C++
mGraphicsPipelineInfo
		.set_primitive_type(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
		.set_input_assembly(...)
		.set_rasterization(...)
		.set_depth_stencil(...)
		.set_blend(...)
		.set_root_signature(...)
		.set_vert_shader(...)
		.set_frag_shader(...)
		.set_render_target(...);

mGraphicsPipeline = directx12::pipeline_state::create(device, mGraphicsPipelineInfo);
```

Support some helpful functions. For example :

```C++
command_queue.wait(...); //wait fence

resource.barrier(command_list, before_state, after_state);
```

## Todo

- Adapters.
- Pipeline State.
- Raytracing.