# directx-wrapper

"directx-wrapper" is a lightweight and independent library to wrap the DirectX Graphics API and make it easy to program with DirectX.

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

C++ style member functions.

```C++
command_list.set_descriptor_heaps({
	descriptor_heap0.get(),
	descriptor_heap1.get(),
	...
});
```

Root signature with named index.

```C++
root_signature_info.add_constants("config", base, space, count);

command_list.set_graphics_constants(root_signature_info.index("config"), {
	value32bit_0, value32bit_1, ..., value32bit_5
});
```

Support some helpful functions. For example :

```C++
command_queue.wait(...); //wait fence

resource.barrier(command_list, before_state, after_state);
```

Raytracing was supported.

### About "fix_resharper_error.hlsl"

For the user with resharper extension. If you enable the resharper extension to work with HLSL, you can include this file to help you write HLSL with better code completion(highlight, warning and so on). 

It is not a good way to support code completion with HLSL. But I think it is helpful.

## Extensions

All extensions are header only file. If you want to use them, you can include the header files of them. But you should ensure the third parties of extensions are installed. For example, if you want to use `dxc` extension, you need ensure your operation system support [dxc](https://github.com/microsoft/DirectXShaderCompiler) and `.dll` files should be your working path.

- `imgui` : a renderer backend of imgui.
- `dxc` : DirectX Shader Compiler.

## Samples

See [realtime-playground](https://github.com/LinkClinton/realtime-playground) and [path-tracing-gpu](https://github.com/LinkClinton/path-tracing-gpu).