#pragma once
class IndexBuffer
{
private:
	unsigned int m_RenderID;
	unsigned int m_Count;
public:
	IndexBuffer();
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();
	IndexBuffer(const IndexBuffer &other) = delete;
	IndexBuffer& operator=(const IndexBuffer& other) = delete;

	// 2. Add Move Constructor
    IndexBuffer(IndexBuffer&& other) noexcept 
        : m_RenderID(other.m_RenderID), m_Count(other.m_Count) 
    {
        other.m_RenderID = 0; // The old object no longer "owns" this ID
        other.m_Count = 0;
    }

    // 3. Add Move Assignment Operator
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;
	void SetData(const unsigned int* data, unsigned int count);
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const {
		return m_Count;
	}
};
