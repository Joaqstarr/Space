#pragma once
template <typename InElementType>
struct TPriorityQueueNode {
    InElementType Element;
    float Priority;

    TPriorityQueueNode(): Priority(0)
    {
    }

    TPriorityQueueNode(InElementType InElement, float InPriority)
    {
        Element = InElement;
        Priority = InPriority;
    }

    bool operator<(const TPriorityQueueNode<InElementType> Other) const
    {
        return Priority < Other.Priority;
    }
};

template <typename InElementType>
class TPriorityQueue {
public:
    TPriorityQueue()
    {
        Array.Heapify();
    }

public:
    // Always check if IsEmpty() before Pop-ing!
    InElementType Pop()
    {
        TPriorityQueueNode<InElementType> Node;
        Array.HeapPop(Node);
        return Node.Element;
    }

    TPriorityQueueNode<InElementType> PopNode()
    {
        TPriorityQueueNode<InElementType> Node;
        Array.HeapPop(Node);
        return Node;
    }

    void Push(InElementType Element, float Priority)
    {
        Array.HeapPush(TPriorityQueueNode<InElementType>(Element, Priority));
    }
    void Remove(InElementType Element)
    {
        
        bool stillContainsElem = true;
        while(stillContainsElem)
        {
            stillContainsElem = false;
            for(int i = 0; i < Array.Num(); ++i)
            {
                if(Array[i].Element == Element)
                {
                    stillContainsElem = true;
                    Array.HeapRemoveAt(i);
                    break;
                }
            }
        }
    }
    bool IsEmpty() const
    {
        return Array.Num() == 0;
    }

    InElementType Top() const
    {
        return Array.HeapTop().Element;
    }
private:
    TArray<TPriorityQueueNode<InElementType>> Array;
};