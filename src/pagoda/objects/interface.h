#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace pagoda::objects
{
////////////////////////////////////////////////////////////
// Forward Declaration
////////////////////////////////////////////////////////////
using ProceduralObjectPtr = std::shared_ptr<class ProceduralObject>;
using InterfacePtr = std::shared_ptr<class Interface>;
using InterfaceWeakPtr = std::weak_ptr<class Interface>;

/**
 * Represents an input or output interface for a ProceduralOperation.
 *
 * Instances ProceduralObject are passed in an out through an Interface.
 * An Interface has a name which allows a ProceduralOperation to identify the origin and destiny of a ProceduralObject
 * and what to do with it. An Interface can be an input or output interface and can accept one or multiple
 * ProceduralObject (its Arity). The Arity of a ProceduralOperations Interfaces specifies how ProceduralObjects are
 * passed into the ProceduralOperation (e.g. By a Node).
 */
class Interface : public std::enable_shared_from_this<Interface>
{
	public:
	/**
	 * Indicates whether an interface is input or output.
	 */
	enum class Type
	{
		Input,  ///< Input interface.
		Output  ///< Output interface.
	};

	/**
	 * Indicates how many ProceduralObjects an interface accepts.
	 */
	enum class Arity
	{
		One,   ///< Accepts a single ProceduralObject.
		Many,  ///< Accepts many ProceduralObjects but only one at a time.
		All    ///< Accepts many ProceduralObjects at once.
	};

	/**
	 * Constructs an Interface with a \p name, \p type and \p arity.
	 */
	Interface(const std::string& name, Type type, Arity arity = Arity::Many);

	////////////////////////////////////////////////////////////
	/// \name Getters and Setters
	////////////////////////////////////////////////////////////

	/**
	 * Returns the Interface name.
	 */
	const std::string& GetName() const;

	/**
	 * Returns the Interface Type.
	 */
	Type GetType() const;

	/**
	 * Returns the Interface Arity.
	 */
	Arity GetArity() const;

	////////////////////////////////////////////////////////////
	/// \name Connection Management
	////////////////////////////////////////////////////////////
	/**
	 * Checks if the interface can be connected to \p i.
	 */
	bool Accepts(const InterfaceWeakPtr& i) const;

	/**
	 * Connects an Interface to this one.
	 * Checks if both Accept before connecting.
	 * @return true if the connection is possible, false otherwise.
	 */
	bool Connect(const InterfaceWeakPtr& interface);

	/**
	 * Disconnects an interface.
	 */
	void Disconnect(const InterfaceWeakPtr& interface);

	/**
	 * Iterates over all connected interfaces.
	 */
	void ConnectedInterfaces(std::function<void(InterfacePtr)> f) const;

	/**
	 * Returns the number of connected Interface.
	 */
	std::size_t ConnectionCount() const;

	////////////////////////////////////////////////////////////
	/// \name Object Accessors
	////////////////////////////////////////////////////////////
	/**
	 * Returns the only ProceduralObject in the interface.
	 * This can only be used if the interface has a Arity::One and only once in each execution.
	 */
	ProceduralObjectPtr Get();

	void Set(ProceduralObjectPtr object);

	/**
	 * Returns the next ProceduralObject in the interface.
	 * This can only be used if the interface has a Arity::Many and only once in each execution.
	 */
	ProceduralObjectPtr GetNext();

	void SetNext(ProceduralObjectPtr object);

	/**
	 * Calls \p f with all ProceduralObject.
	 * This can only be used if the interface has a Arity::All.
	 */
	std::size_t GetAll(std::function<void(ProceduralObjectPtr&)> f);

	void Add(ProceduralObjectPtr object);

	private:
	/**
	 * Hashes a weak pointer to an Interface.
	 */
	struct InterfaceWeakPtrHasher
	{
		size_t operator()(const InterfaceWeakPtr& ptr) const
		{
			auto sh_ptr = ptr.lock();
			return std::hash<std::shared_ptr<Interface>>()(sh_ptr);
		}

		bool operator()(const InterfaceWeakPtr& lhs, const InterfaceWeakPtr& rhs) const
		{
			return lhs.lock().get() == rhs.lock().get();
		}
	};
	/// Holds a collection of weak pointers to Interface objects
	using ConnectionContainer_t = std::unordered_set<InterfaceWeakPtr, InterfaceWeakPtrHasher, InterfaceWeakPtrHasher>;

	std::string m_name;                           ///< The name.
	Type m_type;                                  ///< The Type.
	Arity m_arity;                                ///< Tye Arity.
	std::vector<ProceduralObjectPtr> m_objects;   ///< The objects contained in this interface.
	ConnectionContainer_t m_connectedInterfaces;  ///< The connected Interfaces.
};
}  // namespace pagoda::objects
