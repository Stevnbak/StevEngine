#pragma once
#include <uuidv7.h>
#include <string>
#include <stdint.h>

namespace StevEngine::Utilities {
	/**
	 * @brief UUID-based unique identifier
	 *
	 * Implements UUIDv7 generation for unique object identification.
	 * Provides comparison, serialization and string conversion.
	 */
	class ID {
		public:
			/**
			 * @brief Generate new random UUID
			 */
			ID ();

			/**
			 * @brief Copy constructor
			 * @param other ID to copy
			 */
			ID (const ID& other);

			/**
			 * @brief Create from raw bytes
			 * @param raw 16-byte UUID data
			 */
			ID (uint8_t* raw);

			/**
			 * @brief Create from string representation
			 * @param string UUID string
			 */
			ID (std::string string);

			/**
			 * @brief Get string representation
			 * @return UUID as string
			 */
			std::string GetString() const { return std::string(string); }

			/**
			 * @brief Assignment operator
			 * @param other ID to copy
			 * @return Reference to this
			 */
			ID& operator=(const ID& other);

			/**
			 * @brief Equality comparison
			 * @param other ID to compare
			 * @return true if IDs match
			 */
			bool operator==(const ID& other) const;

			/**
			 * @brief Less than comparison for sorting
			 * @param other ID to compare
			 * @return true if this < other
			 */
			bool operator<(const ID& other) const;

			/**
			 * @brief Compare function for containers
			 * @param lhs First ID
			 * @param rhs Second ID
			 * @return true if lhs < rhs
			 */
			bool operator() (const ID& lhs, const ID& rhs) const;

			/**
			 * @brief Check if ID is null/empty
			 * @return true if null ID
			 */
			bool IsNull() const;

			/** @brief Null/empty ID value */
			static ID empty;

			const uint8_t* GetRaw() const { return raw; }

		private:
			uint8_t raw[16];	 ///< Raw UUID bytes
			std::string string;   ///< String representation
	};
}

template <>
struct std::hash<StevEngine::Utilities::ID>
{
	/**
	 * @brief Hash function for ID
	 * @param k ID to hash
	 * @return Hash value
	 */
	std::size_t operator()(const StevEngine::Utilities::ID& k) const
	{
		return (std::hash<std::string>()(k.GetString()));
	}
};