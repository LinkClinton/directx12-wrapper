#include "command_queue.hpp"

wrapper::directx12::command_queue::command_queue(const ComPtr<ID3D12CommandQueue>& source) : mQueue(source) 
{
}

ID3D12CommandQueue* const* wrapper::directx12::command_queue::get_address_off() const
{
	return mQueue.GetAddressOf();
}

ID3D12CommandQueue* wrapper::directx12::command_queue::operator->() const
{
	return mQueue.Get();
}

ID3D12CommandQueue* wrapper::directx12::command_queue::get() const
{
	return mQueue.Get();
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
