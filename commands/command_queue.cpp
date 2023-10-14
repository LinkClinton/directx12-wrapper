#include "command_queue.hpp"

wrapper::directx12::command_queue::command_queue(const ComPtr<ID3D12CommandQueue>& source) : wrapper_t<ID3D12CommandQueue>(source) 
{
}

void wrapper::directx12::command_queue::execute(const std::vector<graphics_command_list>& command_lists) const
{
	std::vector<ID3D12CommandList*> lists;

	for (auto& list : command_lists) lists.push_back(list.get());

	mWrapperInstance->ExecuteCommandLists(static_cast<UINT>(lists.size()), lists.data());
}

void wrapper::directx12::command_queue::wait(const fence& fence) const
{
	const auto value = fence->GetCompletedValue() + 1;

	mWrapperInstance->Signal(fence.get(), value);

	if (fence->GetCompletedValue() < value) 
	{
		const auto event_handle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

		fence->SetEventOnCompletion(value, event_handle);

		WaitForSingleObject(event_handle, INFINITE);
		CloseHandle(event_handle);
	}
}

wrapper::directx12::command_queue wrapper::directx12::command_queue::create(const device& device)
{
	ComPtr<ID3D12CommandQueue> queue;

	D3D12_COMMAND_QUEUE_DESC desc;

	desc.NodeMask = 0;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Priority = 0;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	
	device->CreateCommandQueue(&desc, IID_PPV_ARGS(queue.GetAddressOf()));

	return command_queue(queue);
}
